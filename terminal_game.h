#pragma once

#include "game.h"

#include <iostream>

int Terminal_Game()
{
    // Info
    std::cout << "Game: C++\n";

    // Game
    Game game;
    bool running = true;

    // Main loop
    while (running) {

        // Render
        switch (game.getState()) {
        case GAME_ACTIVE:
            std::cout << game.toString() << '\n';
            std::cout << "Enter W(UP) A(LEFT) S(DOWN) D(RIGHT) E(ESCAPE): ";
            break;
        case GAME_PAUSE:
            std::cout << "Enter P(PLAY/PAUSE):\n";
            break;
        case GAME_STOP:
            std::cout << "Congratulations! \\o/\n";
            running = false;
            break;
        case GAME_OVER:
            std::cout << "Game Over!\n";
            running = false;
            break;
        }

        // Process Input and Update
        if (running) {
            char entry;
            std::cin >> entry;
            if (entry == 'E' || entry == 'e') {
                game.input(ESCAPE);
            }
            if (entry == 'W' || entry == 'w') {
                game.input(UP);
            }
            if (entry == 'S' || entry == 's') {
                game.input(DOWN);
            }
            if (entry == 'A' || entry == 'a') {
                game.input(LEFT);
            }
            if (entry == 'D' || entry == 'd') {
                game.input(RIGHT);
            }
            if (entry == 'P' || entry == 'p') {
                game.input(PAUSE);
            }
            game.update();
        }
    }

    // Terminate
    std::cout << "Terminate!\n";

    return 0;
}
