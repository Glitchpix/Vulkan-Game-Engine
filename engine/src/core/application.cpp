#include "application.hpp"
#include "logger.hpp"
#include "platform/platform.hpp"
#include "game_types.hpp"

Application::Application(Game* game): mX{game->mX}, mY{game->mY}, mWidth{game->mWidth}, mHeight{game->mHeight}, mName{game->mName}, mGame{game} {
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

    if (!(mGame->initialize())){
        MSG_FATAL("Game failed to initialize!");
        //TODO: Probably throw an exception here too...
    }

    mGame->on_resize(mWidth, mHeight);

}

bool Application::run(){
    while(mRunning) {
        if(!(mPlatform->pumpMessages())){
            mRunning = false;
            break;
        };

        if(!mSuspended) {
            if (!(mGame->update(0.0f))){
                MSG_FATAL("Game update failed! Shutting down.");
                mRunning = false;
                break;
            }
            if (!(mGame->render(0.0f))){
                MSG_FATAL("Game render failed! Shutting down.");
                mRunning = false;
                break;
            }
        }
        mPlatform->sleep(100);
    }

    mRunning = false;

    mPlatform->shutdown();
    
    return true;
}

