#pragma once

#include "defines.hpp"
#include <array>
#include <memory>
#include <string>


class MemoryManager {
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

    DLL_EXPORT std::string get_usage();

    DLL_EXPORT static void* zero(void* block, size_t size);
    DLL_EXPORT static void* copy(void* dest, const void* source, size_t size);
    DLL_EXPORT static void* set(void* dest, int value, size_t size);

private:
    struct AllocationInfo {
        size_t bytesAllocated{};
        std::string tagString;
    };
    struct Stats {
        size_t total_allocated{};
        std::array<AllocationInfo, tag::MEMORY_TAG_MAX_TAGS> tagged_allocations{
            {{.tagString = "UNKNOWN"}, {.tagString = "TEST"}}};
    };
    Stats mStats{};
};