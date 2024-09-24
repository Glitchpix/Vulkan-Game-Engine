#pragma once
#include "defines.hpp"
#include <cstdarg>
#include <cstdio>
#include <platform/platform.hpp>
#include <sstream>
#include <string>


#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if K_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

enum LogLevel {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
};

class Logger {
public:
    DLL_EXPORT static bool init_logging();
    static void shutdown_logging();
    static std::string log_output(LogLevel level, const char* format, ...) __attribute__((format(printf, 2, 3))) {
        const std::string prepend_level[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
        bool is_error = level < LOG_LEVEL_WARN;

        std::ostringstream stringStream;
        stringStream << prepend_level[level];

        std::string result;
        // TODO: see if there's a safe way to do this with variadic templates
        // Will complain about literal string format (-Wformat security), due to format not being checked.
        va_list args = nullptr;
        va_list args_copy = nullptr;

        va_start(args, format);
        va_copy(args_copy, args);

        int len = vsnprintf(nullptr, 0, format, args);
        if (len < 0) {
            va_end(args_copy);
            va_end(args);
            throw std::runtime_error("vsnprintf error");
        }

        if (len > 0) {
            result.resize(len);
            vsnprintf(result.data(), len + 1, format, args_copy);
        }

        va_end(args_copy);
        va_end(args);

        stringStream << result << '\n';

        std::string message = stringStream.str();

        if (is_error) {
            Platform::consoleWriteError(message, level);
        } else {
            Platform::consoleWriteError(message, level);
        }
        return message;
    }
};

#define MSG_FATAL(format, ...) Logger::log_output(LOG_LEVEL_FATAL, format, __VA_ARGS__);

#ifndef MSG_ERROR
#define MSG_ERROR(format, ...) Logger::log_output(LOG_LEVEL_ERROR, format, __VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#define MSG_WARN(format, ...) Logger::log_output(LOG_LEVEL_WARN, format, __VA_ARGS__);
#else
#define MSG_WARN(format, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define MSG_INFO(format, ...) Logger::log_output(LOG_LEVEL_INFO, format, __VA_ARGS__);
#else
#define MSG_INFO(format, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define MSG_DEBUG(format, ...) Logger::log_output(LOG_LEVEL_DEBUG, format, __VA_ARGS__);
#else
#define MSG_DEBUG(format, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define MSG_TRACE(...) Logger::log_output(LOG_LEVEL_TRACE, __VA_ARGS__);
#else
#define MSG_TRACE(...)
#endif