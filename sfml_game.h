#include<SFML/Graphics.hpp>

#include "game.h"

#include <iostream>

using namespace std;

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
        cout << "Failed to load font!" << endl;
        return EXIT_FAILURE;
    }
    text.setFont(font);

    // Info
    cout << "Game  : C++" << endl;
    cout << "OpenGL: " << window.getSettings().majorVersion << "."
         << window.getSettings().minorVersion << endl;
    cout << "SFML  : " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << endl;

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
            text.setString("Level: " + to_string(game.getLevel()) + "  " +
                           "Score: " + to_string(game.getScore()));
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
        text.setCharacterSize(min(width, height));
        text.setFillColor(sf::Color::Black);

        sf::RectangleShape rect(sf::Vector2f(width, height));
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                sf::Color color;
                switch (game.getScene()[y][x]) {
                case PLAYER:
                    color = sf::Color::Blue;
                    break;
                case ENEMY:
                    color = sf::Color::Cyan;
                    break;
                case STAR:
                    color = sf::Color::Yellow;
                    break;
                case OBSTACLE:
                    color = sf::Color::Green;
                    break;
                case COLLISION:
                    color = sf::Color::Red;
                    break;
                default:
                    color = sf::Color::Black;
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
    cout << "Terminate!" << endl;

    return 0;
}
