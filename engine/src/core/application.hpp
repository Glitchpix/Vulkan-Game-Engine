#pragma once

#include "defines.hpp"
#include <memory>

class Platform;

class Application {
private:
    short mX;
    short mY;
    short mWidth;
    short mHeight;

    char* mName;

    bool mRunning;
    bool mSuspended;
    std::unique_ptr<Platform> mPlatform;

public:
    DLL_EXPORT Application(short x, short y, short width, short height, char* name);
    ~Application() = default;

    DLL_EXPORT bool run();
};