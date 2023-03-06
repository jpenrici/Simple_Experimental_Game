#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"

#include <iostream>
#include <thread>

int GLFW_Game(unsigned int screen_width, unsigned int screen_height)
{
    // GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!\n";
        return EXIT_FAILURE;
    }

    // Window
    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height,
                                          "Game", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window!\n" ;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Context
    glfwMakeContextCurrent(window);

    // Callback
    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow * window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // Glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return EXIT_FAILURE;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screen_width, 0, screen_height, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Info
    std::cout << "Game  : C++\n";
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << '\n';
    std::cout << "GLFW  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    // Game
    Game game;
    bool running = true;

    // Main loop
    while (running && !glfwWindowShouldClose(window)) {
        // Wait
        std::this_thread::sleep_for(std::chrono::milliseconds(60));

        // Update
        game.update();
        switch (game.getState()) {
        case GAME_ACTIVE:
        case GAME_PAUSE:
        case GAME_STOP:
        case GAME_OVER:
            break;
        }

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int rows = game.getScene().size();
        int cols = game.getScene().front().size();
        float width = screen_width / cols ;
        float height = screen_height / rows;

        GLfloat r, g, b;
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
                GLfloat colour[] = {
                    r, g, b,
                    r, g, b,
                    r, g, b,
                    r, g, b,
                };
                float x = col * width;
                float y = screen_height - (row * height) - height;
                float vertices[] = {
                    x, y, 0.0,
                    x + width, y, 0.0,
                    x + width, y + height, 0.0,
                    x, y + height, 0.0
                };
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_COLOR_ARRAY);
                glVertexPointer(3, GL_FLOAT, 0, vertices);
                glColorPointer(3, GL_FLOAT, 0, colour);
                glDrawArrays(GL_QUADS, 0, 4);
                glDisableClientState(GL_COLOR_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);
            }
        }

        glfwSwapBuffers(window);

        // Process Input
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            running = false;
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            game.input(UP);
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            game.input(DOWN);
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            game.input(LEFT);
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            game.input(RIGHT);
        }
        else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            game.input(PAUSE);
        }
        else {
            game.input(NONE);
        }
    }

    // Terminate
    glfwSetWindowShouldClose(window, true);
    glfwTerminate();
    std::cout << "Terminate!\n";

    return 0;
}
