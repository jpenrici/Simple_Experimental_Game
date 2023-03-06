#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum GameState { GAME_ACTIVE, GAME_PAUSE, GAME_STOP, GAME_OVER };
enum Action { NONE, UP, RIGHT, DOWN, LEFT, PAUSE, ESCAPE };

// Symbols for .lvl file
const char SPACE     = '.';
const char PLAYER    = 'P';
const char ENEMY     = 'E';
const char STAR      = 'S';
const char OBSTACLE  = 'O';
const char COLLISION = 'X';

// Resource directory
const char RESOURCES[] = "Resources/";

class Game {

public:
    Game();
    ~Game() {};

    Game(Game const &) = delete;
    Game &operator=(Game const &) = delete;

    void update();
    void input(int action);
    void restart();
    int getScore();
    int getLevel();
    GameState getState();
    std::vector<std::string> getScene();
    std::string toString();

private:

    struct Position {
        int x, y;
    };

    struct GameObject {

        GameObject() {};
        GameObject(int x, int y);

        Position offset(int x, int y);
        void setPosition(int x, int y);

        Position position;
    };

    GameState state;
    GameObject player;
    std::vector<GameObject> enemies;

    std::vector<std::string> scene;
    std::vector<std::vector<std::string> > levels;
    int action, level, score, stars;

    void setLevel();
    std::vector<std::string> standard();
    std::vector<std::string> loadLevel(std::string resourceFile);
};
