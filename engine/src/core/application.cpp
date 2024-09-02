#include "application.hpp"
#include "logger.hpp"
#include "platform/platform.hpp"

DLL_EXPORT Application::Application(short x, short y, short width, short height, char* name): mX{x}, mY{y}, mWidth{width}, mHeight{height}, mName{name} {
    //TODO: Enforce single instance?

    // Initialize systems
    Logger::init_logging();

    //TODO: Test code, remove
    MSG_FATAL("very %s", "bad");
    MSG_ERROR("an error of %f", 3.4);
    MSG_WARN("okay");
    MSG_INFO("good");
    MSG_DEBUG("detailed");
    MSG_TRACE("even");

    mRunning = true;
    mSuspended = false;

    mPlatform = std::make_unique<Platform>();
    if (!(mPlatform->startup(mName, mX, mY, mWidth, mHeight))) {
        MSG_FATAL("Failed to start platform window!");
        //TODO: Probably throw an exception here too...
    }
}

bool DLL_EXPORT Application::run(){
    while(mRunning) {
        if(!(mPlatform->pumpMessages())){
            mRunning = false;
        };
        mPlatform->sleep(100);
    }

    mRunning = false;

    mPlatform->shutdown();
    
    return true;
}

