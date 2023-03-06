#include "glfw_game.h"
#include "sdl_game.h"
#include "sfml_game.h"
#include "terminal_game.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    // Test
    if (argc == 2) {
        if (argv[1] == std::string("sfml")) {
            SFML_Game(800, 600);
        }
        else if (argv[1] == std::string("sdl")) {
            SDL2_Game(800, 600);
        }
        else if (argv[1] == std::string("glfw")) {
            GLFW_Game(800, 600);
        }
        else if (argv[1] == std::string("terminal")) {
            Terminal_Game();
        }
        else {
            std::cout << "use: Game [glfw|sdl|sfml|terminal]" << std::endl;
        }
    }
    else {
        SFML_Game(800, 600);
    }

    return 0;
}
