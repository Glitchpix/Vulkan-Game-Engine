#include "e_memory.hpp"
#include "logger.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//TODO: Translate to something sensible in C++
// Idea: shared or unique ptr and make use of size_of, handle delete with custom deleter
/*
https://stackoverflow.com/questions/39288891/why-is-shared-ptrvoid-legal-while-unique-ptrvoid-is-ill-formed
https://stackoverflow.com/questions/19053351/how-do-i-use-a-custom-deleter-with-a-stdunique-ptr-member
https://en.cppreference.com/w/cpp/memory/unique_ptr
*/

namespace Memory {
    struct Stats{
        size_t total_allocated;
        size_t tagged_allocations[MEMORY_TAG_MAX_TAGS];
    };
    static Stats stats;

    static const char* tag_strings[MEMORY_TAG_MAX_TAGS] = {
        "UNKNOWN",
        "TEST   "};
}

void Memory::initialize(){
    zero(&Memory::stats, sizeof(Memory::stats));
}

void Memory::shutdown(){

}

void* Memory::allocate(size_t size, memory_tag tag){
    if (tag == MEMORY_TAG_UNKNOWN) {
        MSG_WARN("Allocate called with MEMORY_TAG_UNKNOWN, re-call with correct tag.")
    }
    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    void* block = malloc(size);
    memset(block, 0, size);
    return block;
}

void Memory::free_block(void* block, size_t size, memory_tag tag){
    if (tag == MEMORY_TAG_UNKNOWN) {
        MSG_WARN("Free called with MEMORY_TAG_UNKNOWN, re-call with correct tag.")
    }
    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    free(block);
}

void* Memory::zero(void* block, size_t size){
    return memset(block, 0, size);
}

void* Memory::copy(void* dest, const void* source, size_t size){
    return memcpy(dest, source, size);
}

void* Memory::set(void* dest, int value, size_t size){
    return memset(dest, value, size);
}

char* Memory::get_usage(){
    const long gibibyte = 1024*1024*1024;
    const long mebibyte = 1024*1024;
    const long kibibyte = 1024;

    const size_t bufferSize = 8000;
    char buffer[bufferSize] = "System memory use (tagged): \n";
    size_t offset = strlen(buffer);
    for (size_t i = 0; i < MEMORY_TAG_MAX_TAGS; ++i){
        char unit[4] = "XiB";
        float amount = 1.0f;
        if (stats.tagged_allocations[i] >= gibibyte){
            unit[0] = 'G';
            amount = stats.tagged_allocations[i] / (float)gibibyte;
        } else if (stats.tagged_allocations[i] >= mebibyte){
            unit[0] = 'M';
            amount = stats.tagged_allocations[i] / (float)mebibyte;
        } else if (stats.tagged_allocations[i] >= kibibyte){
            unit[0] = 'K';
            amount = stats.tagged_allocations[i] / (float)kibibyte;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float) stats.tagged_allocations[i];
        }

        size_t length = snprintf(buffer + offset, bufferSize, "  %s: %.2f%s\n", tag_strings[i], amount, unit);
        offset += length;
    }
    char* outString = _strdup(buffer);
    return outString;
}