#pragma once
#include "defines.hpp"
#include <format>
#include <platform/platform.hpp>
#include <sstream>
#include <string>
#include <string_view>


constexpr bool LOG_WARN_ENABLED = true;
constexpr bool LOG_INFO_ENABLED = true;
constexpr bool LOG_DEBUG_ENABLED = true;
constexpr bool LOG_TRACE_ENABLED = false;

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
    template <class... Args>
    static std::string log_output(LogLevel level, std::string_view format, Args&&... args) {
        const std::string prepend_level[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
        bool is_error = level < LOG_LEVEL_WARN;

        std::ostringstream stringStream;
        stringStream << prepend_level[level];

        std::string formatted_message;
        if constexpr (sizeof...(args) > 0) {
            formatted_message = std::vformat(format, std::make_format_args(args...));
        } else {
            formatted_message = std::string(format);
        }

        stringStream << formatted_message << '\n';
        std::string message = stringStream.str();

        if (is_error) {
            Platform::consoleWriteError(message, level);
        } else {
            Platform::consoleWriteError(message, level);
        }
        return message;
    }
};

template <typename... Args>
constexpr std::string MSG_FATAL(std::string_view format, Args&&... args) {
    return Logger::log_output(LOG_LEVEL_FATAL, format, std::forward<Args>(args)...);
}

template <typename... Args>
constexpr std::string MSG_ERROR(std::string_view format, Args&&... args) {
    return Logger::log_output(LOG_LEVEL_ERROR, format, std::forward<Args>(args)...);
}

template <typename... Args>
constexpr std::string MSG_WARN(std::string_view format, Args&&... args) {
    if constexpr (LOG_WARN_ENABLED) {
        return Logger::log_output(LOG_LEVEL_WARN, format, std::forward<Args>(args)...);
    } else {
        return "";
    }
}

template <typename... Args>
constexpr std::string MSG_INFO(std::string_view format, Args&&... args) {
    if constexpr (LOG_INFO_ENABLED) {
        return Logger::log_output(LOG_LEVEL_INFO, format, std::forward<Args>(args)...);
    } else {
        return "";
    }
}

template <typename... Args>
constexpr std::string MSG_DEBUG(std::string_view format, Args&&... args) {
    if constexpr (LOG_DEBUG_ENABLED) {
        return Logger::log_output(LOG_LEVEL_DEBUG, format, std::forward<Args>(args)...);
    } else {
        return "";
    }
}

template <typename... Args>
constexpr std::string MSG_TRACE(std::string_view format, Args&&... args) {
    if constexpr (LOG_TRACE_ENABLED) {
        return Logger::log_output(LOG_LEVEL_TRACE, format, std::forward<Args>(args)...);
    } else {
        return "";
    }
}