#pragma once

#include "core/event.hpp"
#include "defines.hpp"
#include <memory>
#include <string>


class Platform;
class Game;
class InputHandler;
class Clock;
class Renderer;

class Application {
public:
    DLL_EXPORT Application(Game& game, EventManager& eventManager);
    DLL_EXPORT ~Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    DLL_EXPORT bool run();

private:
    short mX{0};
    short mY{0};
    short mWidth{0};
    short mHeight{0};

    std::string mName;

    bool mRunning{false};
    bool mSuspended{false};

    std::unique_ptr<Clock> mClock;
    std::unique_ptr<Platform> mPlatform;
    Game& mGame;
    EventManager& mEventManager;
    std::unique_ptr<InputHandler> mInputHandler;
    std::unique_ptr<Renderer> mRenderer;

    static bool on_event(EventManager::EventCode code, void* sender, void* listener, EventManager::Context context);
    static bool on_key(EventManager::EventCode code, void* sender, void* listener, EventManager::Context context);
    static bool on_mouse_move(EventManager::EventCode code, void* sender, void* listener,
                              EventManager::Context context);
};