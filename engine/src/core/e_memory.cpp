#include "e_memory.hpp"
#include "logger.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>


// Idea: shared or unique ptr and make use of size_of, handle delete with custom deleter
/*
https://stackoverflow.com/questions/39288891/why-is-shared-ptrvoid-legal-while-unique-ptrvoid-is-ill-formed
https://stackoverflow.com/questions/19053351/how-do-i-use-a-custom-deleter-with-a-stdunique-ptr-member
https://en.cppreference.com/w/cpp/memory/unique_ptr
https://stackoverflow.com/questions/62312973/how-to-initialize-unique-ptr-from-raw-c-heap-memory-pointer

https://stackoverflow.com/questions/12264701/shared-ptr-with-malloc-and-free

Future implementation idea: Practical Memory Pool Based Allocators For Modern C++
https://www.youtube.com/watch?v=l14Zkx5OXr4

TODO: Lambda functions may cause issues with many calls to allocate, investigate this.
*/

MemoryManager::MemoryManager() {
    MSG_TRACE("MemoryManager: %p created", static_cast<void*>(this));
}

void MemoryManager::shutdown() {
    // TODO: Destructor instead perhaps?
}

auto MemoryManager::allocate(const size_t size, const tag tag) -> std::shared_ptr<void> {
    if (tag == tag::MEMORY_TAG_UNKNOWN) {
        MSG_WARN("Allocate called with MEMORY_TAG_UNKNOWN, re-call with correct tag.")
    }
    mStats.total_allocated += size;
    mStats.tagged_allocations[tag] += size;

    std::shared_ptr<void> block(malloc(size), [this, size, tag](void* block) { this->free_block(block, size, tag); });
    return block;
}

void MemoryManager::free_block(void* block, const size_t size, const tag tag) {
    if (tag == tag::MEMORY_TAG_UNKNOWN) {
        MSG_WARN("Free called with MEMORY_TAG_UNKNOWN, re-call with correct tag.")
    }
    mStats.total_allocated -= size;
    mStats.tagged_allocations[tag] -= size;

    free(block);

    MSG_DEBUG("Block: %p with size: %zu and tag: %s freed", block, size, tag_strings[tag]);
}

char* MemoryManager::get_usage() {
    const long gibibyte = 1024 * 1024 * 1024;
    const long mebibyte = 1024 * 1024;
    const long kibibyte = 1024;

    const size_t bufferSize = 8000;
    char buffer[bufferSize] = "System memory use (tagged): \n";
    size_t offset = strlen(buffer);
    for (size_t i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "XiB";
        float amount = 1.0F;
        if (mStats.tagged_allocations[i] >= gibibyte) {
            unit[0] = 'G';
            amount = mStats.tagged_allocations[i] / static_cast<float>(gibibyte);
        } else if (mStats.tagged_allocations[i] >= mebibyte) {
            unit[0] = 'M';
            amount = mStats.tagged_allocations[i] / static_cast<float>(mebibyte);
        } else if (mStats.tagged_allocations[i] >= kibibyte) {
            unit[0] = 'K';
            amount = mStats.tagged_allocations[i] / static_cast<float>(kibibyte);
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = static_cast<float>(mStats.tagged_allocations[i]);
        }

        size_t length = snprintf(buffer + offset, bufferSize, "  %s: %.2f%s\n", tag_strings[i], amount, unit);
        offset += length;
    }
    char* outString = _strdup(buffer);
    return outString;
}

void* MemoryManager::zero(void* block, size_t size) {
    return memset(block, 0, size);
}

void* MemoryManager::copy(void* dest, const void* source, size_t size) {
    return memcpy(dest, source, size);
}

void* MemoryManager::set(void* dest, int value, size_t size) {
    return memset(dest, value, size);
}