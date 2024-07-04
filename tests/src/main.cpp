#include <core/logger.hpp>

int main(void) {
    MSG_FATAL("very", " bad");
    MSG_ERROR("not ", "so ", "bad");
    MSG_WARN("okay");
    MSG_INFO("good");
    MSG_DEBUG("detailed");
    MSG_TRACE("even ", "more ", "detailed");
    return 0;
}