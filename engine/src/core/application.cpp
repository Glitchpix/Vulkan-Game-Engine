#include "application.hpp"
#include "core/clock.hpp"
#include "core/input.hpp"
#include "core/logger.hpp"
#include "game_types.hpp"
#include "platform/platform.hpp"
#include "renderer/renderer.hpp"


Application::Application(Game &game, EventManager &eventManager)
    : mX{game.mX}, mY{game.mY}, mWidth{game.mWidth}, mHeight{game.mHeight},
      mName{game.mName}, mRunning{true}, mGame{game}, mEventManager{eventManager} {
    // TODO: Enforce single instance?
    mInputHandler = std::make_unique<InputHandler>(mEventManager);

    mEventManager.register_event(
        EventManager::EventCode::EVENT_CODE_APPLICATION_QUIT, this,
        Application::on_event);
    mEventManager.register_event(EventManager::EventCode::EVENT_CODE_KEY_PRESSED,
                                 this, Application::on_key);
    mEventManager.register_event(EventManager::EventCode::EVENT_CODE_KEY_RELEASED,
                                 this, Application::on_key);
    mEventManager.register_event(EventManager::EventCode::EVENT_CODE_MOUSE_MOVED, this, Application::on_mouse_move);

    mPlatform = std::make_unique<Platform>(mInputHandler.get());
    if (!(mPlatform->startup(mName, mX, mY, mWidth, mHeight))) {
        MSG_FATAL("Failed to start platform window!");
        // TODO: Probably throw an exception here too...
        // Move these into functions to lessen error checking here
    }

    mClock = std::make_unique<Clock>(mPlatform.get());

    mRenderer = std::make_unique<Renderer>(game.mName, mPlatform.get());

    if (!(mGame.initialize())) {
        MSG_FATAL("Game failed to initialize!");
        // TODO: Probably throw an exception here too...
    }
    MSG_TRACE("Game: {:p} initialized", static_cast<void *>(&mGame));

    mGame.on_resize(mWidth, mHeight);
    MSG_TRACE("Application: {:p} created", static_cast<void *>(this));
}

Application::~Application() = default;

bool Application::run() {
    // const float framesPerSecondTarget = 60.0F;
    // f64 targetFrameSeconds = 1.0F / framesPerSecondTarget;
    mClock->start();
    f64 deltaTime = 0;
    while (mRunning) {
        if (!(mPlatform->pumpMessages())) {
            mRunning = false;
            break;
        };

        if (!mSuspended) {
            mClock->update();
            // START OF FRAME

            if (!(mGame.update(deltaTime))) {
                MSG_FATAL("Game update failed! Shutting down.");
                mRunning = false;
                break;
            }
            if (!(mGame.render(deltaTime))) {
                MSG_FATAL("Game render failed! Shutting down.");
                mRunning = false;
                break;
            }

            Renderer::RenderPacket packet{};
            packet.deltaTime = deltaTime;
            mRenderer->draw_frame(packet);

            // END OF FRAME
            mInputHandler->update(deltaTime);
            mClock->update();
            deltaTime = mClock->delta_time();
            // f64 timeLeftAfterTargetFrame = targetFrameSeconds - deltaTime;
            // TODO allow state to sleep here depending on timeLeft
            const auto SleepTime = 20;
            mPlatform->sleep(SleepTime);
            MSG_TRACE("Frame and input delta: {:f}", deltaTime);
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

bool Application::on_event(EventManager::EventCode code, void * /*unused*/,
                           void *listener, EventManager::Context /*unused*/) {
    auto *instance = static_cast<Application *>(listener);
    switch (code) {
        case EventManager::EventCode::EVENT_CODE_APPLICATION_QUIT:
            MSG_TRACE(
                "EVENT_CODE_APPLICATION_QUIT recieved, shutting down application");
            instance->mRunning = false;
            return true;
        default:
            break;
    }
    return false;
}

bool Application::on_key(EventManager::EventCode code, void * /*unused*/,
                         void *listener, EventManager::Context context) {
    //TODO: Remove/Replace some debug information here
    auto *instance = static_cast<Application *>(listener);
    if (code == EventManager::EventCode::EVENT_CODE_KEY_PRESSED) {
        auto keyCode = static_cast<InputHandler::Key>(context.i16[0]);
        if (keyCode == InputHandler::Key::KEY_ESCAPE) {
            EventManager::Context quitContext{};
            instance->mEventManager.fire_event(
                EventManager::EventCode::EVENT_CODE_APPLICATION_QUIT, listener,
                quitContext);
            return true;
        }
        if (keyCode == InputHandler::Key::KEY_A) {
            MSG_DEBUG("Explicit 'A' Key pressed");
        } else {
            MSG_DEBUG("'{:c}' Key pressed", static_cast<char>(keyCode));
        }
    } else if (code == EventManager::EventCode::EVENT_CODE_KEY_RELEASED) {
        auto keyCode = static_cast<InputHandler::Key>(context.i16[0]);
        if (keyCode == InputHandler::Key::KEY_B) {
            MSG_DEBUG("Explicit 'B' Key released");
        } else {
            MSG_DEBUG("'{:c}' Key released", static_cast<char>(keyCode));
        }
    }
    return false;
}


bool Application::on_mouse_move(EventManager::EventCode code, void * /*unused*/,
                                void * /*unused*/, EventManager::Context context) {
    //TODO: Remove/Replace some debug information here
    // auto *instance = static_cast<Application *>(listener);
    if (code == EventManager::EventCode::EVENT_CODE_MOUSE_MOVED) {
        i16 x = context.i16[0];
        i16 y = context.i16[1];
        MSG_TRACE("Mouse move processed: ({}, {})", x, y);
    }
    return false;
}
