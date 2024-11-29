#pragma once
#include "doctest.h"
#include "math/vec.hpp"

TEST_CASE("Test Vec2 trivial equality") {
    Vec2 firstVec{{2}, {2}};
    Vec2 secondVec{{2}, {2}};

    CHECK(firstVec == secondVec);
}