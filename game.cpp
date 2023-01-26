#include "game.h"

using namespace std;

Game::Game() : level(0), score(0), stars(0), action(NONE), state(GAME_STOP)
{
    // Check .lvl files
    for (const auto &entry : filesystem::directory_iterator(RESOURCES)) {
        string resourceFile = entry.path();
        if (resourceFile.substr(resourceFile.length() - 4) == ".lvl") {
            vector<string> newLevel = loadLevel(resourceFile);
            if (!newLevel.empty()) {
                levels.push_back(newLevel);
            }
        }
    }
    if (levels.empty()) {
        levels.push_back(standard());
        cout << "Warning: Missing .lvl files! Use default." << endl;
    }

    // setup
    setLevel();
}

void Game::setLevel()
{
    // Scene
    scene = levels[level];
    state = GAME_PAUSE;
    action = NONE;

    stars = 0;
    enemies.clear();
    for (int y = 0; y < scene.size(); ++y) {
        for (int x = 0; x < scene[y].size(); ++x) {
            switch (scene[y][x]) {
            case PLAYER:
                player.setPosition(x, y);
                break;
            case ENEMY:
                enemies.push_back(GameObject(x, y));
                break;
            case OBSTACLE:
                break;
            case STAR:
                stars++;
                break;
            default:
                scene[y][x] = SPACE;
                break;
            }
        }
    }

    // Info
    cout << "Total levels : " << levels.size() << endl;
    cout << "Current Level: " << level << endl;
    cout << "Total Enemies: " << enemies.size() << endl;
    cout << "Toral Stars  : " << stars << endl;
    cout << "-------------- " << endl;
}

void Game::update()
{
    int Ymax = scene.size();
    int Xmax = scene.front().size();

    if (state == GAME_ACTIVE) {
        auto p = player.offset((action == RIGHT) - (action == LEFT),
                               (action == DOWN) - (action == UP));
        if (p.x >= 0 && p.x < Xmax && p.y >= 0 && p.y < Ymax) {
            switch (scene[p.y][p.x]) {
            case STAR:
                score++;
                if (--stars < 1) {
                    if (++level >= levels.size()) {
                        state = GAME_STOP;
                    }
                    else {
                        setLevel();
                    }
                }
            case SPACE:
                scene[player.position.y][player.position.x] = SPACE;
                scene[p.y][p.x] = PLAYER;
                player.setPosition(p.x, p.y);
                break;
            case ENEMY:
                scene[player.position.y][player.position.x] = SPACE;
                scene[p.y][p.x] = COLLISION;
                state = GAME_OVER;
                break;
            }
        }
    }

    if (state == GAME_ACTIVE) {
        for (auto &enemy : enemies) {
            int d[] = {-1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1};
            auto e = enemy.offset(d[rand() % 10], d[rand() % 10]);
            if (e.x >= 0 && e.x < Xmax && e.y >= 0 && e.y < Ymax) {
                switch (scene[e.y][e.x]) {
                case SPACE:
                    scene[enemy.position.y][enemy.position.x] = SPACE;
                    scene[e.y][e.x] = ENEMY;
                    enemy.setPosition(e.x, e.y);
                    break;
                case PLAYER:
                    scene[enemy.position.y][enemy.position.x] = SPACE;
                    scene[e.y][e.x] = COLLISION;
                    state = GAME_OVER;
                    break;
                }
            }
            if (state == GAME_OVER) {
                break;
            }
        }
    }
}

void Game::input(int action)
{
    switch (action) {
    case UP:
    case DOWN:
    case LEFT:
    case RIGHT:
        this->action = action;
        break;
    case PAUSE:
        this->action = NONE;
        if (state == GAME_ACTIVE) {
            state = GAME_PAUSE;
        }
        else if (state == GAME_PAUSE) {
            state = GAME_ACTIVE;
        }
        break;
    case ESCAPE:
        state = GAME_OVER;
    default:
        this->action = NONE;
        break;
    }
}

void Game::restart()
{
    level = 0;
    score = 0;
    setLevel();
}

int Game::getScore()
{
    return score;
}

int Game::getLevel()
{
    return level;
}

GameState Game::getState()
{
    return state;
}

vector<string> Game::getScene()
{
    return scene;
}

string Game::toString()
{
    string text = "Score: " + to_string(score) + " Level: " + to_string(level) + "\n";
    for (const auto &line : scene) {
        text += line + "\n";
    }
    return text;
}

vector<string> Game::loadLevel(string resourceFile)
{
    // Entry
    cout << "Load: " << resourceFile << endl;

    // Check
    if (resourceFile.substr(resourceFile.length() - 4) != ".lvl") {
        cout << "Error: Invalid file!" << endl;
        return {};
    }
    if (!filesystem::exists(resourceFile)) {
        cout << "Error: File not found!" << endl;
        return {};
    }

    // New scene
    vector<string> newScene{};

    // Load text file
    bool isValid = true;
    ifstream ifs(resourceFile);
    try {
        string text{};
        string line{};
        if (ifs.is_open()) {
            while (getline(ifs, line)) {
                if (!newScene.empty()) {
                    if (line.length() != newScene.back().length()) {
                        cout << "Error: Invalid .lvl file structure!" << endl;
                        newScene = {};
                        break;
                    }
                }
                newScene.push_back(line);
                text.append(line);
            }
        }
        // Check symbols
        if (isValid) {
            int players = count_if(text.begin(), text.end(), [](char c) {
                return c == PLAYER;
            });
            int enemies = count_if(text.begin(), text.end(), [](char c) {
                return c == ENEMY;
            });
            int stars = count_if(text.begin(), text.end(), [](char c) {
                return c == STAR;
            });
            if (players != 1 || enemies == 0 || stars == 0) {
                cout << "Error: Failed to read game object symbols!" << endl;
                newScene = {};
            }
        }
    }
    catch (...) {
        cout << "Error: There was something wrong opening .lvl file!!" << endl;
        newScene = {};
    }
    ifs.close();

    return newScene;
}

vector<string> Game::standard()
{
    // P(Player), E(Enemy), O(Obstacle), S(Star), .(Space)
    // Default example: 10 x 10
    return {
        "OOOOOOOOOO",
        "O.P......O",
        "O........O",
        "O...O....O",
        "O...O....O",
        "O...O....O",
        "O.....S..O",
        "O.E......O",
        "O..E.....O",
        "OOOOOOOOOO"
    };
}

Game::GameObject::GameObject(int x, int y)
{
    position = {x, y};
}

Game::Position Game::GameObject::offset(int x, int y)
{
    return Game::Position {position.x + x, position.y + y};
}

void Game::GameObject::setPosition(int x, int y)
{
    position = {x, y};
}
