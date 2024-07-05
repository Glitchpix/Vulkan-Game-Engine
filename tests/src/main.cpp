#include <core/logger.hpp>
#include <core/asserts.hpp>

int main(void) {
    MSG_FATAL("very %s", "bad");
    MSG_ERROR("an error of %f", 3.4);
    MSG_WARN("okay");
    MSG_INFO("good");
    MSG_DEBUG("detailed");
    MSG_TRACE("even");

    ENGINE_ASSERT_MESSAGE(1 == 0, "1 does not equal 0!");
    return 0;
}