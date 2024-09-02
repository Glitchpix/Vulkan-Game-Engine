#pragma once

#include "defines.hpp"
#include <string>
#include <memory>

class Platform {
public:
    struct State {};
    std::unique_ptr<State> mState;
    double mClock_frequency;

    Platform();
    ~Platform() = default;

    bool startup(const std::string& application_name,
                         int x, int y, unsigned int width, unsigned int height);
    void shutdown();
    bool pumpMessages();

    static void consoleWrite(const std::string& message, unsigned char colour);
    static void consoleWriteError(const std::string& message, unsigned char colour);

    double getAbsoluteTime();
    void sleep(std::size_t ms);
};