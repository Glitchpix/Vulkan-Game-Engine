#pragma once
#include "core/application.hpp"
#include "core/logger.hpp"
#include "game_types.hpp"
#include "core/e_memory.hpp"

extern bool create_game(Game*);

int main() {

    memory::initialize();
    
    // Test memory
    //TODO: Remove this
    const size_t blockSize = 1024*5;
    void* testBlockA = memory::allocate(blockSize, MEMORY_TAG_TEST);
    memory::zero(testBlockA, blockSize);
    memory::set(testBlockA, 1, blockSize);

    void* testBlockB = memory::allocate(blockSize, MEMORY_TAG_TEST);
    memory::zero(testBlockB, blockSize);
    memory::set(testBlockB, 1, blockSize);

    MSG_INFO(memory::get_usage());

    MSG_INFO("Freeing blocks...")

    memory::free_block(testBlockA, blockSize, MEMORY_TAG_TEST);
    memory::free_block(testBlockB, blockSize, MEMORY_TAG_TEST);

    MSG_INFO(memory::get_usage());

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

    memory::shutdown();
    
    return 0;
}
