#pragma once
#include "core/application.hpp"
#include "core/logger.hpp"
#include "game_types.hpp"
#include "core/e_memory.hpp"

extern bool create_game(Game*);

int main() {

    MemoryManager memoryManager{};
    
    // Test memory
    //TODO: Remove this
    const size_t blockSize = 1024*5;
    {
        std::shared_ptr<void> testBlockA = memoryManager.allocate(blockSize, MemoryManager::tag::MEMORY_TAG_TEST);
        MemoryManager::zero(testBlockA.get(), blockSize);
        MemoryManager::set(testBlockA.get(), 1, blockSize);

        std::shared_ptr<void> testBlockB = memoryManager.allocate(blockSize, MemoryManager::tag::MEMORY_TAG_TEST);
        MemoryManager::zero(testBlockB.get(), blockSize);
        MemoryManager::set(testBlockB.get(), 1, blockSize);

        MSG_INFO(memoryManager.get_usage());

        MSG_INFO("Freeing blocks...")
    }

    MSG_INFO(memoryManager.get_usage());

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

    memoryManager.shutdown();
    
    return 0;
}
