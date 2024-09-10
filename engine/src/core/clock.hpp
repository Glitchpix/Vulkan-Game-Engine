#pragma once

#include "defines.hpp"

class Platform;

class Clock {
public:
    Clock(Platform* platform);
    void start();
    void stop();
    void update();
private:
    f64 mStartTime{0};
    f64 mElapsedTime{0};
    Platform* mPlatform;
};