#include <entry.hpp>
#include "game.hpp"

bool create_game(Game* game){
    //TODO: Construct game directly instead?
    game->mX = 100;
    game->mY = 100;
    game->mWidth = 1280;
    game->mHeight = 720;
    game->mName = "Test Engine";
    game->initialize = GameFunctions::initialize;
    game->update = GameFunctions::update;
    game->render = GameFunctions::render;
    game->on_resize = GameFunctions::on_resize;
    return true;
}