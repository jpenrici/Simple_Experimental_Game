#pragma once

#include<SFML/Graphics.hpp>

#include "game.h"

#include <iostream>

int SFML_Game(unsigned int screen_width, unsigned int screen_height)
{
    // SFML
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Game",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(100, 100));
    window.setFramerateLimit(60);

    sf::Font font;
    sf::Text text;
    if (!font.loadFromFile("Resources/NotoSans-Black.ttf")) {
        std::cout << "Failed to load font!\n";
        return EXIT_FAILURE;
    }
    text.setFont(font);

    // Info
    std::cout << "Game  : C++\n";
    std::cout << "OpenGL: " << window.getSettings().majorVersion << "."
              << window.getSettings().minorVersion << '\n';
    std::cout << "SFML  : " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << '\n';

    // Game
    Game game;
    bool running = true;

    // Main loop
    while (running && window.isOpen()) {
        // Wait
        sf::sleep(sf::milliseconds(60));

        // Process Input
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    running = false;
                }
                if (event.key.code == sf::Keyboard::Up) {
                    game.input(UP);
                }
                if (event.key.code == sf::Keyboard::Down) {
                    game.input(DOWN);
                }
                if (event.key.code == sf::Keyboard::Left) {
                    game.input(LEFT);
                }
                if (event.key.code == sf::Keyboard::Right) {
                    game.input(RIGHT);
                }
                if (event.key.code == sf::Keyboard::Space) {
                    if (game.getState() == GAME_PAUSE) {
                        game.input(PAUSE);
                    }
                    else if (game.getState() == GAME_STOP ||
                             game.getState() == GAME_OVER) {
                        game.restart();
                    }
                }
            }
            else {
                game.input(NONE);
            }
        }

        // Update
        game.update();
        switch (game.getState()) {
        case GAME_ACTIVE:
            text.setString("Level: " + std::to_string(game.getLevel()) + "  " +
                           "Score: " + std::to_string(game.getScore()));
            break;
        case GAME_PAUSE:
            text.setString("Press SPACE to start and ARROWS to move.");
            break;
        case GAME_STOP:
            text.setString("\\o/ Congratulations! Press SPACE.");
            break;
        case GAME_OVER:
            text.setString("Game Over! Press SPACE.");
            break;
        }

        // Render
        window.clear();
        int rows = game.getScene().size();
        int cols = game.getScene().front().size();
        float width = window.getSize().x  / cols;
        float height = (window.getSize().y - text.getLocalBounds().height) / rows;

        text.setPosition(width, text.getLocalBounds().height);
        text.setCharacterSize(std::min(width, height));
        text.setFillColor(sf::Color::Black);

        sf::RectangleShape rect(sf::Vector2f(width, height));
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                sf::Color color;
                switch (game.getScene()[y][x]) {
                case PLAYER:
                    color = sf::Color::Blue;   // RGB(0,0,255)
                    break;
                case ENEMY:
                    color = sf::Color::Cyan;   // RGB(0,255,255)
                    break;
                case STAR:
                    color = sf::Color::Yellow; // RGB(255,255,0)
                    break;
                case OBSTACLE:
                    color = sf::Color::Green;  // RGB(0,255,0)
                    break;
                case COLLISION:
                    color = sf::Color::Red;    // RGB(255,0,0)
                    break;
                default:
                    color = sf::Color::Black;  // RGB(0,0,0)
                    break;
                }
                rect.setFillColor(color);
                rect.setPosition(x * width, y * height + text.getLocalBounds().height);
                window.draw(rect);
            }
        }
        window.draw(text);
        window.display();
    }

    // Terminate
    window.close();
    std::cout << "Terminate!\n";

    return 0;
}
