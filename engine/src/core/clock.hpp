#pragma once

#include "defines.hpp"

class Platform;

class Clock {
public:
    Clock(Platform* platform);
    void start();
    void reset();
    void update();
    [[nodiscard]] f64 delta_time() const;
private:
    f64 mStartTime{0};
    f64 mElapsedTime{0};
    f64 mCurrentTime{0};
    f64 mPreviousTime{0};
    Platform* mPlatform;
};