#pragma once
#include <string>
#include <utility>

class Game {
public:
    short mX{0};
    short mY{0};
    short mWidth{0};
    short mHeight{0};

    std::string mName;


    Game() = default;
    Game(short x, short y, short width, short height, std::string name) : mX{x}, mY{y}, mWidth{width}, mHeight{height}, mName{std::move(name)} {};

    bool (*initialize)(){nullptr};
    bool (*update)(double deltaTime){nullptr};
    bool (*render)(double deltaTime){nullptr};

    void (*on_resize)(short width, short height){nullptr};
};