#pragma once
#include "core/application.hpp"
#include "core/logger.hpp"
#include "game_types.hpp"

extern bool create_game(Game*);

int main() {
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
    
    return 0;
}
