#pragma once

#define ENGINE_PLATFORM_WINDOWS 1

#include "defines.hpp"
#include <memory>
#include <string>

class InputHandler;
class Platform {
public:
    struct State {
        State(const State&) = default;
        State(State&&) = delete;
        State& operator=(const State&) = default;
        State& operator=(State&&) = delete;
        State() = default;
        virtual ~State() = default;
    };

    Platform(const Platform&) = delete;
    Platform(Platform&&) = delete;
    Platform& operator=(const Platform&) = delete;
    Platform& operator=(Platform&&) = delete;
    Platform(InputHandler* InputHandler);
    ~Platform() = default;

    bool startup(const std::string& application_name,
                 int x, int y, int width, int height);
    void shutdown();
    bool pumpMessages();

    DLL_EXPORT static void consoleWrite(const std::string& message, unsigned char colour);
    DLL_EXPORT static void consoleWriteError(const std::string& message, unsigned char colour);

    [[nodiscard]] double getAbsoluteTime() const;
    [[nodiscard]] State* getState();
    static void sleep(std::size_t ms);

private:
    std::unique_ptr<State> mState;
    double mClock_frequency{};
    InputHandler* mInputHandler;
};