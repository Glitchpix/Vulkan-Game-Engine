#pragma once

#include "defines.hpp"
#include <memory>

class MemoryManager{
public:
    enum tag {
        MEMORY_TAG_UNKNOWN,
        MEMORY_TAG_TEST,

        MEMORY_TAG_MAX_TAGS
    };

    DLL_EXPORT MemoryManager();

    DLL_EXPORT void initialize();
    DLL_EXPORT void shutdown();

    DLL_EXPORT std::shared_ptr<void> allocate(size_t size, tag tag);
    DLL_EXPORT void free_block(void* block, size_t size, tag tag);

    DLL_EXPORT char* get_usage();

    DLL_EXPORT static void* zero(void* block, size_t size);
    DLL_EXPORT static void* copy(void* dest, const void* source, size_t size);
    DLL_EXPORT static void* set(void* dest, int value, size_t size);

private:
    struct Stats{
        size_t total_allocated;
        size_t tagged_allocations[tag::MEMORY_TAG_MAX_TAGS];
    };
    Stats mStats{};

    const char* tag_strings[tag::MEMORY_TAG_MAX_TAGS] = {
        "UNKNOWN",
        "TEST   "
    };
};