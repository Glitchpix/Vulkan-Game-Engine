#include <core/logger.hpp>

int main(void) {
    Logger::log_output(LOG_LEVEL_FATAL, "cool", "test");
    return 0;
}