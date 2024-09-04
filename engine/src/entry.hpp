#pragma once
#include "core/application.hpp"
#include "core/logger.hpp"
#include "game_types.hpp"
#include "core/e_memory.hpp"

extern bool create_game(Game*);

int main() {

    Memory::initialize();
    
    // Test memory
    //TODO: Remove this
    const size_t blockSize = 1024*5;
    void* testBlockA = Memory::allocate(blockSize, MEMORY_TAG_TEST);
    Memory::zero(testBlockA, blockSize);
    Memory::set(testBlockA, 1, blockSize);

    void* testBlockB = Memory::allocate(blockSize, MEMORY_TAG_TEST);
    Memory::zero(testBlockB, blockSize);
    Memory::set(testBlockB, 1, blockSize);

    MSG_INFO(Memory::get_usage());

    MSG_INFO("Freeing blocks...")

    Memory::free_block(testBlockA, blockSize, MEMORY_TAG_TEST);
    Memory::free_block(testBlockB, blockSize, MEMORY_TAG_TEST);

    MSG_INFO(Memory::get_usage());

    Game game{};
    if(!create_game(&game)){
        MSG_FATAL("Game creation failed!");
        return -1;
    }

    if(!game.initialize || !game.update || !game.render || !game.on_resize){
        MSG_FATAL("Not all game function pointers are assigned!")
        return -2;
    }

    Application app{&game};

    if(!app.run()){
        MSG_FATAL("Application did not shutdown gracefully!")
        return -3;
    };

    Memory::shutdown();
    
    return 0;
}
