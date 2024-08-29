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

    DLL_EXPORT bool startup(const std::string& application_name,
                         int x, int y, unsigned int width, unsigned int height);
    DLL_EXPORT void shutdown();
    DLL_EXPORT bool pumpMessages();

    DLL_EXPORT static void consoleWrite(const std::string& message, unsigned char colour);
    DLL_EXPORT static void consoleWriteError(const std::string& message, unsigned char colour);

    DLL_EXPORT double getAbsoluteTime();
    DLL_EXPORT void sleep(std::size_t ms);
};