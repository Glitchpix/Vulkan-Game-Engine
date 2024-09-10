#pragma once
#include "defines.hpp"
#include <platform/platform.hpp>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <cstdarg>
#include <sstream>

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if K_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} log_level;

class Logger {
    public:
        DLL_EXPORT static bool init_logging();
        static void shutdown_logging();
        template<class ...Args>
        static std::string log_output(log_level level, const char* format, Args&&... args) {
            const std::string prepend_level[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
            bool is_error = level < LOG_LEVEL_WARN;

            std::ostringstream stringStream;
            stringStream << prepend_level[level];

            const unsigned int max_length = 32000;
            char buffer[max_length];
            snprintf(buffer, max_length, format, args...);

            stringStream << buffer << '\n';

            std::string message = stringStream.str();

            if (is_error) {
                Platform::consoleWriteError(message.c_str(), level);
            } else {
                Platform::consoleWriteError(message.c_str(), level);
            }
            return message;
        }
};

#define MSG_FATAL(format, ...) Logger::log_output(LOG_LEVEL_FATAL, format, ##__VA_ARGS__);

#ifndef MSG_ERROR
#define MSG_ERROR(format, ...) Logger::log_output(LOG_LEVEL_ERROR, format, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#define MSG_WARN(format, ...) Logger::log_output(LOG_LEVEL_WARN, format, ##__VA_ARGS__);
#else
#define MSG_WARN(format, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define MSG_INFO(format, ...) Logger::log_output(LOG_LEVEL_INFO, format, ##__VA_ARGS__);
#else
#define MSG_INFO(format, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define MSG_DEBUG(format, ...) Logger::log_output(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__);
#else
#define MSG_DEBUG(format, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define MSG_TRACE(...) Logger::log_output(LOG_LEVEL_TRACE, ##__VA_ARGS__);
#else
#define MSG_TRACE(...)
#endif