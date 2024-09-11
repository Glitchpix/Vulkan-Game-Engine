#include "application.hpp"
#include "game_types.hpp"
#include "platform/platform.hpp"
#include "core/input.hpp"
#include "core/logger.hpp"
#include "core/clock.hpp"

Application::Application(Game &game, EventManager &eventManager)
    : mX{game.mX}, mY{game.mY}, mWidth{game.mWidth}, mHeight{game.mHeight},
      mName{game.mName}, mGame{game}, mEventManager{eventManager}
{
    // TODO: Enforce single instance?
    mInputHandler = std::make_unique<InputHandler>(mEventManager);

    mRunning = true;
    mSuspended = false;

    mEventManager.register_event(
        EventManager::EventCode::EVENT_CODE_APPLICATION_QUIT, this,
        Application::on_event);
    mEventManager.register_event(EventManager::EventCode::EVENT_CODE_KEY_PRESSED,
                                 this, Application::on_key);
    mEventManager.register_event(EventManager::EventCode::EVENT_CODE_KEY_RELEASED,
                                 this, Application::on_key);

    mPlatform = std::make_unique<Platform>(mInputHandler.get());
    if (!(mPlatform->startup(mName, mX, mY, mWidth, mHeight)))
    {
        MSG_FATAL("Failed to start platform window!");
        // TODO: Probably throw an exception here too...
    }

    mClock = std::make_unique<Clock>(mPlatform.get());

    if (!(mGame.initialize()))
    {
        MSG_FATAL("Game failed to initialize!");
        // TODO: Probably throw an exception here too...
    }
    MSG_TRACE("Game: %p initialized", &mGame);

    mGame.on_resize(mWidth, mHeight);
    MSG_TRACE("Application: %p created", this);
}

Application::~Application() = default;

bool Application::run()
{
    const float framesPerSecondTarget = 60.0f;
    f64 targetFrameSeconds = 1.0f / framesPerSecondTarget;
    mClock->start();
    f64 deltaTime = 0;
    while (mRunning)
    {
        if (!(mPlatform->pumpMessages()))
        {
            mRunning = false;
            break;
        };

        if (!mSuspended)
        {
            if (!(mGame.update(deltaTime)))
            {
                MSG_FATAL("Game update failed! Shutting down.");
                mRunning = false;
                break;
            }
            if (!(mGame.render(deltaTime)))
            {
                MSG_FATAL("Game render failed! Shutting down.");
                mRunning = false;
                break;
            }
            mClock->update();
            deltaTime = mClock->delta_time();

            f64 timeLeftAfterTargetFrame = targetFrameSeconds - deltaTime;

            // TODO allow state to sleep here depending on timeLeft
            mInputHandler->update(deltaTime);
            MSG_TRACE("Frame and input delta: %f", deltaTime);
        }
    }

    mRunning = false;

    mEventManager.unregister_event(
        EventManager::EventCode::EVENT_CODE_APPLICATION_QUIT, this,
        Application::on_event);
    mEventManager.unregister_event(EventManager::EventCode::EVENT_CODE_KEY_PRESSED,
                                   this, Application::on_key);
    mEventManager.unregister_event(EventManager::EventCode::EVENT_CODE_KEY_RELEASED,
                                   this, Application::on_key);

    mPlatform->shutdown();

    return true;
}

bool Application::on_event(EventManager::EventCode code, void *sender,
                           void *listener, EventManager::Context context)
{
    Application *instance = static_cast<Application*>(listener);
    switch (code)
    {
    case EventManager::EventCode::EVENT_CODE_APPLICATION_QUIT:
        MSG_TRACE(
            "EVENT_CODE_APPLICATION_QUIT recieved, shutting down application");
        instance->mRunning = false;
        return true;
    }
    return false;
}

bool Application::on_key(EventManager::EventCode code, void *sender,
                         void *listener, EventManager::Context context)
{
    //TODO: Remove/Replace some debug information here
    Application *instance = static_cast<Application*>(listener);
    if (code == EventManager::EventCode::EVENT_CODE_KEY_PRESSED)
    {
        InputHandler::Key keyCode = static_cast<InputHandler::Key>(context.i16[0]);
        if (keyCode == InputHandler::Key::KEY_ESCAPE)
        {
            EventManager::Context quitContext{};
            instance->mEventManager.fire_event(
                EventManager::EventCode::EVENT_CODE_APPLICATION_QUIT, listener,
                quitContext);
            return true;
        }
        else if (keyCode == InputHandler::Key::KEY_A)
        {
            MSG_DEBUG("Explicit 'A' Key pressed");
        }
        else
        {
            MSG_DEBUG("'%c' Key pressed", keyCode);
        }
    }
    else if (code == EventManager::EventCode::EVENT_CODE_KEY_RELEASED)
    {
        InputHandler::Key keyCode = static_cast<InputHandler::Key>(context.i16[0]);
        if (keyCode == InputHandler::Key::KEY_B)
        {
            MSG_DEBUG("Explicit 'B' Key released");
        }
        else
        {
            MSG_DEBUG("'%c' Key released", keyCode);
        }
    }
    return false;
}
