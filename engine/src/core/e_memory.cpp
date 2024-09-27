#include "e_memory.hpp"
#include "logger.hpp"
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


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
    MSG_TRACE("MemoryManager: {:p} created", static_cast<void*>(this));
}

void MemoryManager::shutdown() {
    // TODO: Destructor instead perhaps?
}

auto MemoryManager::allocate(const size_t size, const tag tag) -> std::shared_ptr<void> {
    if (tag == tag::MEMORY_TAG_UNKNOWN) {
        MSG_WARN("Allocate called with MEMORY_TAG_UNKNOWN, re-call with correct tag.");
    }
    mStats.total_allocated += size;
    mStats.tagged_allocations.at(tag).bytesAllocated += size;

    std::shared_ptr<void> block(malloc(size), [this, size, tag](void* block) { this->free_block(block, size, tag); });
    MSG_DEBUG("Block: {:p} with size: {} and tag: {} allocated", block.get(), size, mStats.tagged_allocations.at(tag).tagString.c_str());
    return block;
}

void MemoryManager::free_block(void* block, const size_t size, const tag tag) {
    if (tag == tag::MEMORY_TAG_UNKNOWN) {
        MSG_WARN("Free called with MEMORY_TAG_UNKNOWN, re-call with correct tag.");
    }
    mStats.total_allocated -= size;
    mStats.tagged_allocations.at(tag).bytesAllocated -= size;

    free(block);

    MSG_DEBUG("Block: {:p} with size: {} and tag: {} freed", block, size, mStats.tagged_allocations.at(tag).tagString.c_str());
}

std::string MemoryManager::get_usage() {
    const long gibibyte = 1024 * 1024 * 1024;
    const long mebibyte = 1024 * 1024;
    const long kibibyte = 1024;

    std::ostringstream stringStream;
    stringStream << "System memory use (tagged): \n";

    // Define unit thresholds and names
    const std::vector<std::pair<size_t, std::string>> units = {
        {gibibyte, "GiB"},
        {mebibyte, "MiB"},
        {kibibyte, "KiB"},
        {1, "B"}  // Fallback to bytes
    };

    for (const auto& tagged_allocation : mStats.tagged_allocations) {
        double amount{};
        auto tagString = tagged_allocation.tagString;
        std::string unit = "B";

        // Find the appropriate unit
        for (const auto& [threshold, name] : units) {
            if (tagged_allocation.bytesAllocated >= threshold) {
                amount = static_cast<double>(tagged_allocation.bytesAllocated) / static_cast<double>(threshold);
                unit = name;
                break;
            }
        }

        // Append formatted string to the stream
        stringStream << "  " << std::left << std::setw(10) << tagString << ": " << std::setprecision(2) << amount << unit << "\n";
    }

    return stringStream.str();
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