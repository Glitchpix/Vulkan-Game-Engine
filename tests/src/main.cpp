#include <core/asserts.hpp>
#include <core/logger.hpp>

// TODO: Test
#include <platform/platform.hpp>

int main(void) {
    MSG_FATAL("very %s", "bad");
    MSG_ERROR("an error of %f", 3.4);
    MSG_WARN("okay");
    MSG_INFO("good");
    MSG_DEBUG("detailed");
    MSG_TRACE("even");

    Platform* window{};
    if (window->startup("Engine Testbed", 100, 100, 1280, 720)) {
        while(true) {
            window->pumpMessages();
            window->sleep(100);
        }
    }
    window->shutdown();
    return 0;
}