#pragma once
#include "defines.hpp"
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

class DLL_EXPORT Logger {
    public:
        static bool init_logging();
        static void shutdown_logging();
        template<class ...Args>
        static void log_output(log_level level, std::string message, Args... args) {
            const std::string prepend_level[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
            bool is_error = level < LOG_LEVEL_WARN;
            //TODO handle this

            std::ostringstream stringStream;
            stringStream << prepend_level[level];

            for(const auto& arg : {args...}){
                stringStream << arg;
            }
            stringStream << "\n";

            message = stringStream.str();

            std::cout << message;
        }
};

// #define MSG_FATAL(message, ...) Logger::log_output(LOG_LEVEL_FATAL, message, ...);

// #ifndef MSG_ERROR
// #define MSG_ERROR(message, ...) Logger::log_output(LOG_LEVEL_ERROR, message, ...);
// #endif

// #if LOG_WARN_ENABLED == 1
// #define MSG_WARN(message, ...) Logger::log_output(LOG_LEVEL_WARN, message, ...);
// #else
// #define MSG_WARN(message, ...)
// #endif

// #if LOG_INFO_ENABLED == 1
// #define MSG_INFO(message, ...) Logger::log_output(LOG_LEVEL_INFO, message, ...);
// #else
// #define MSG_INFO(message, ...)
// #endif

// #if LOG_DEBUG_ENABLED == 1
// #define MSG_DEBUG(message, ...) Logger::log_output(LOG_LEVEL_DEBUG, message, ...);
// #else
// #define MSG_DEBUG(message, ...)
// #endif

// #if LOG_TRACE_ENABLED == 1
// #define MSG_TRACE(message, ...) Logger::log_output(LOG_LEVEL_TRACE, message, ...);
// #else
// #define MSG_TRACE(message, ...)
// #endif