#pragma once

#include "defines.hpp"
#include "event.hpp"
#include <memory>

class Platform;
class Game;
class InputHandler;

class Application {
public:
    DLL_EXPORT Application(Game& game, EventManager& eventManager);
    DLL_EXPORT ~Application();

    DLL_EXPORT bool run();
private:
    short mX{0};
    short mY{0};
    short mWidth{0};
    short mHeight{0};

    char* mName{""};

    bool mRunning{false};
    bool mSuspended{false};
    std::unique_ptr<Platform> mPlatform;
    std::unique_ptr<Game> mGame;
    EventManager& mEventManager;
    std::unique_ptr<InputHandler> mInputHandler;

    static bool on_event(EventManager::EventCode code, void* sender, void* listener, EventManager::Context context);
    static bool on_key(EventManager::EventCode code, void* sender, void* listener, EventManager::Context context);
};