#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"

int SDL2_Game(unsigned int screen_width, unsigned int screen_height)
{
    // Info
    std::cout << "Game : C++\n";
    std::cout << "SDL  : " << SDL_MAJOR_VERSION << "." << SDL_MINOR_VERSION << '\n';

    // SDL2
    SDL_Window *pWindow = NULL;
    SDL_Renderer *pRenderer = NULL;
    SDL_Surface *pFontSurface = NULL;
    SDL_Texture *pText_Texture = NULL;
    TTF_Font *pFont = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Failed to initialize SDL!\n";
        return EXIT_FAILURE;
    }
    pWindow = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               screen_width, screen_height,
                               SDL_WINDOW_SHOWN);
    if (pWindow == NULL) {
        std::cout << "Failed to initialize SDL!\n";
        return EXIT_FAILURE;
    }
    pRenderer =  SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

    if (TTF_Init() >= 0) {
        pFont = TTF_OpenFont("Resources/NotoSans-Black.ttf", 24);
    }

    SDL_Event e;

    // Game
    Game game;
    bool running = true;

    // Main loop
    while (running) {
        // Wait
        SDL_Delay(60);

        // Process Input
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_UP:
                    game.input(UP);
                    break;
                case SDLK_DOWN:
                    game.input(DOWN);
                    break;
                case SDLK_LEFT:
                    game.input(LEFT);
                    break;
                case SDLK_RIGHT:
                    game.input(RIGHT);
                    break;
                case SDLK_SPACE:
                    if (game.getState() == GAME_PAUSE) {
                        game.input(PAUSE);
                    }
                    else if (game.getState() == GAME_STOP ||
                             game.getState() == GAME_OVER) {
                        game.restart();
                    }
                    break;
                }
            }
            else {
                game.input(NONE);
            }
        }

        // Update
        game.update();
        std::string msg;
        switch (game.getState()) {
        case GAME_ACTIVE:
            msg = "Level: " + std::to_string(game.getLevel()) + "  " +
                    "Score: " + std::to_string(game.getScore());
            break;
        case GAME_PAUSE:
            msg = "Press SPACE to start and ARROWS to move.";
            break;
        case GAME_STOP:
            msg = "\\o/ Congratulations! Press SPACE.";
            break;
        case GAME_OVER:
            msg = "Game Over! Press SPACE.";
            break;
        }

        // Render
        SDL_RenderClear(pRenderer);

        int rows = game.getScene().size();
        int cols = game.getScene().front().size();
        int width = screen_width / cols ;
        int height = screen_height / rows;
        int r, g, b;
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                switch (game.getScene()[row][col]) {
                case PLAYER:
                    r = 0;
                    g = 0;
                    b = 255;
                    break;
                case ENEMY:
                    r = 0;
                    g = 255;
                    b = 255;
                    break;
                case STAR:
                    r = 255;
                    g = 255;
                    b = 0;
                    break;
                case OBSTACLE:
                    r = 0;
                    g = 255;
                    b = 0;
                    break;
                case COLLISION:
                    r = 255;
                    g = 0;
                    b = 0;
                    break;
                default:
                    r = 0;
                    g = 0;
                    b = 0;
                    break;
                }

                SDL_Rect rect = {col * width, row * height, width, height};
                SDL_SetRenderDrawColor(pRenderer, r, g, b, 255);
                SDL_RenderFillRect(pRenderer, &rect);
            }
        }

        if (pFont != NULL) {
            SDL_Color color = { 0, 0, 0 };
            pFontSurface = TTF_RenderText_Solid(pFont, msg.c_str(), color);
            if (pFontSurface != NULL) {
                pText_Texture = SDL_CreateTextureFromSurface(pRenderer, pFontSurface);
            }
            if (pText_Texture != NULL) {
                SDL_Rect dest = {0, 0, pFontSurface->w, pFontSurface->h};
                SDL_RenderCopy(pRenderer, pText_Texture, NULL, &dest);
            }
        }

        SDL_RenderPresent(pRenderer);
    }

    // Terminate
    TTF_CloseFont(pFont);
    TTF_Quit();
    pFont = NULL;

    SDL_FreeSurface(pFontSurface);
    pFontSurface = NULL;

    SDL_DestroyTexture(pText_Texture);
    pText_Texture = NULL;

    SDL_DestroyRenderer(pRenderer);
    pRenderer = NULL;

    SDL_DestroyWindow(pWindow);
    pWindow = NULL;

    SDL_Quit();
    std::cout << "Terminate!\n";

    return 0;
}
