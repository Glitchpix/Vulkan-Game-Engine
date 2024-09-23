#pragma once
#include "core/asserts.hpp"

#define VK_CHECK(expr)                     \
    {                                      \
        ENGINE_ASSERT(expr == VK_SUCCESS); \
    }