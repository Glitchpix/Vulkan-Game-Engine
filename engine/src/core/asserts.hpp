#pragma once

#include "defines.hpp"

#define ENGINE_ASSERTIONS_ENABLED

#ifdef ENGINE_ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

namespace assertions{
    DLL_EXPORT void reportAssertionFailure(const char* expression, const char* format, const char* file, size_t line);
}

#define ENGINE_ASSERT(expr)                                         \
    {                                                               \
        if (expr) {                                                 \
        } else {                                                    \
            assertions::reportAssertionFailure(#expr, "", __FILE__, __LINE__);  \
            debugBreak();                                           \
        }                                                           \
    }                                                               

#define ENGINE_ASSERT_MESSAGE(expr, format)                             \
    {                                                                   \
        if (expr) {                                                     \
        } else {                                                        \
            assertions::reportAssertionFailure(#expr, format, __FILE__, __LINE__);  \
            debugBreak();                                               \
        }                                                               \
    }                                                                   

#ifdef _DEBUG
#define ENGINE_ASSERT_DEBUG(expr)                                   \
    {                                                               \
        if (expr) {                                                 \
        } else {                                                    \
            assertions::reportAssertionFailure(#expr, "", __FILE__, __LINE__);  \
            debugBreak();                                           \
        }                                                           \
    }                                                               
#else
#define ENGINE_ASSERT_DEBUG(expr)
#endif

#else
#define ENGINE_ASSERT(expr)
#define ENGINE_ASSERT_MESSAGE(expr, format)
#define ENGINE_ASSERT_DEBUG(expr)
#endif