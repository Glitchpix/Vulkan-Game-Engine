#include "logger.hpp"
#include "asserts.hpp"

DLL_EXPORT void assertions::reportAssertionFailure(const char* expression, const char* format, const char* file, size_t line) {
    Logger::log_output(LOG_LEVEL_FATAL, "Assertion Fail: %s, message '%s' in file: %s, line: %zu\n", expression, format, file, line);
}

bool Logger::init_logging() {
    return true;
}

void Logger::shutdown_logging() {
}