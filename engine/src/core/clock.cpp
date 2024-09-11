#include "clock.hpp"
#include "logger.hpp"
#include "platform/platform.hpp"

Clock::Clock(Platform* platform) : mPlatform{platform} {
    MSG_TRACE("Clock: %p created", this);
}

void Clock::start(){
    mStartTime = mPlatform->getAbsoluteTime();
    mCurrentTime = mStartTime;
    mPreviousTime = mCurrentTime;
    mElapsedTime = 0;
}

void Clock::reset(){
    mStartTime = 0;
    mElapsedTime = 0;
    mCurrentTime = 0;
    mPreviousTime = 0;
}

void Clock::update() {
    if (mStartTime > 0) {
        mPreviousTime = mCurrentTime;
        mCurrentTime = mPlatform->getAbsoluteTime();
        mElapsedTime = mCurrentTime - mStartTime;
    }
}

// Time since last update() call
f64 Clock::delta_time() {
    return mCurrentTime - mPreviousTime;
}