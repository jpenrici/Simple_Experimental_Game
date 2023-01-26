#include "game.h"

#include <iostream>

using namespace std;

int Terminal_Game()
{
    // Info
    cout << "Game: C++" << endl;

    // Game
    Game game;
    bool running = true;

    // Main loop
    while (running) {

        // Render
        switch (game.getState()) {
        case GAME_ACTIVE:
            cout << game.toString() << endl;
            cout << "Enter W(UP) A(LEFT) S(DOWN) D(RIGHT) E(ESCAPE): ";
            break;
        case GAME_PAUSE:
            cout << "Enter P(PLAY/PAUSE): " << endl;
            break;
        case GAME_STOP:
            cout << "Congratulations! \\o/" << endl;
            running = false;
            break;
        case GAME_OVER:
            cout << "Game Over!" << endl;
            running = false;
            break;
        }

        // Process Input and Update
        if (running) {
            char entry;
            cin >> entry;
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
    cout << "Terminate!" << endl;

    return 0;
}
