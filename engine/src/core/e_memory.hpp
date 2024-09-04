#pragma once

#include "defines.hpp"

enum memory_tag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_TEST,

    MEMORY_TAG_MAX_TAGS
};

namespace Memory {
    DLL_EXPORT void initialize();
    DLL_EXPORT void shutdown();

    DLL_EXPORT void* allocate(size_t size, memory_tag tag);
    DLL_EXPORT void free_block(void* block, size_t size, memory_tag tag);
    DLL_EXPORT void* zero(void* block, size_t size);
    DLL_EXPORT void* copy(void* dest, const void* source, size_t size);
    DLL_EXPORT void* set(void* dest, int value, size_t size);
    DLL_EXPORT char* get_usage();
}