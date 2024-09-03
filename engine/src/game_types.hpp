#pragma once
#include <memory>

class Game {
public:
    short mX{0};
    short mY{0};
    short mWidth{0};
    short mHeight{0};

    char* mName{""};


    Game() = default;
    Game(short x, short y, short width, short height, char* name) : mX{x}, mY{y}, mWidth{width}, mHeight{height}, mName{name} {};
    
    bool (*initialize)();
    bool (*update)(float deltaTime);
    bool (*render)(float deltaTime);

    void (*on_resize)(short width, short height);
};