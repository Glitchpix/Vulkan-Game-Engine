#pragma once

#include <defines.hpp>
#include <game_types.hpp>

//TODO: Implement some of these
namespace GameFunctions{
    bool initialize(){
        return true;
    };
    bool update(double deltaTime){
        return true;
    };
    bool render(double deltaTime){
        return true;
    };
    void on_resize(short width, short height){
    };
}

