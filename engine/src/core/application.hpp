#pragma once

#include "defines.hpp"
#include <memory>

class Platform;
class Game;

class Application {
private:
    short mX{0};
    short mY{0};
    short mWidth{0};
    short mHeight{0};

    char* mName{""};

    bool mRunning;
    bool mSuspended;
    std::unique_ptr<Platform> mPlatform;
    std::unique_ptr<Game> mGame;

public:
    DLL_EXPORT Application(Game* game);
    ~Application() = default;

    DLL_EXPORT bool run();
};