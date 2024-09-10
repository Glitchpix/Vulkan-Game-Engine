#include "clock.hpp"
#include "logger.hpp"
#include "platform/platform.hpp"

Clock::Clock(Platform* platform) : mPlatform{platform} {
    MSG_TRACE("Clock: %p created", this);
}

void Clock::start(){
    mStartTime = mPlatform->getAbsoluteTime();
    mElapsedTime = 0;
}

void Clock::stop(){
    mStartTime = 0;
    mElapsedTime = 0;
}

void Clock::update() {
    if (mStartTime > 0) {
        mElapsedTime = mPlatform->getAbsoluteTime() - mStartTime;
    }
}