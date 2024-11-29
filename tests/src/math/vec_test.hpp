#pragma once
#include "doctest.h"
#include "math/vec.hpp"

TEST_SUITE("Vec2") {
    TEST_CASE("Equality - No initialized values") {
        Vec2 firstVec;
        Vec2 secondVec;

        CHECK(firstVec != secondVec);
    }
    TEST_CASE("Equality - Default initialized values") {
        Vec2 firstVec{};
        Vec2 secondVec{};

        CHECK(firstVec == secondVec);
    }
    TEST_CASE("Equality - Mixed-sign values") {
        Vec2 firstVec{{2}, {-2}};
        Vec2 secondVec{{2}, {-2}};

        CHECK(firstVec == secondVec);
    }
    TEST_CASE("Not Equal - Mixed-sign values") {
        Vec2 firstVec{};
        Vec2 secondVec{};

        SUBCASE("First value differs") {
            firstVec.x = -1;
            CHECK(firstVec != secondVec);
        }
        SUBCASE("Second value differs") {
            firstVec.y = -1;
            CHECK(firstVec != secondVec);
        }
    }
    TEST_CASE("Negation - Mixed-sign values") {
        Vec2 firstVec{};
        firstVec.x = 2;
        firstVec.y = -2;
        firstVec = -firstVec;
        CHECK(firstVec.x == -2);
        CHECK(firstVec.y == 2);
    }
    TEST_CASE("Addition - Mixed-sign values") {
        Vec2 firstVec{{2}, {-2}};
        Vec2 secondVec{{2}, {-2}};

        Vec2 resultVec = firstVec + secondVec;
        CHECK(resultVec.x == 4);
        CHECK(resultVec.y == -4);
    }
    TEST_CASE("Subtraction - Mixed-sign values") {
        Vec2 firstVec{{2}, {-2}};
        Vec2 secondVec{{2}, {-2}};

        Vec2 resultVec = firstVec - secondVec;
        CHECK(resultVec.x == 0);
        CHECK(resultVec.y == 0);
    }
}

TEST_SUITE("Vec3") {
    TEST_CASE("Equality - No initialized values") {
        Vec3 firstVec;
        Vec3 secondVec;

        CHECK(firstVec != secondVec);
    }
    TEST_CASE("Equality - Default initialized values") {
        Vec3 firstVec{};
        Vec3 secondVec{};

        CHECK(firstVec == secondVec);
    }
    TEST_CASE("Equality - Mixed-sign values") {
        Vec3 firstVec{{2}, {-2}, {2}};
        Vec3 secondVec{{2}, {-2}, {2}};

        CHECK(firstVec == secondVec);
    }
    TEST_CASE("Not Equal - Mixed-sign values") {
        Vec3 firstVec{};
        Vec3 secondVec{};

        SUBCASE("First value differs") {
            firstVec.x = -1;
            CHECK(firstVec != secondVec);
        }
        SUBCASE("Second value differs") {
            firstVec.y = -1;
            CHECK(firstVec != secondVec);
        }
        SUBCASE("Third value differs") {
            firstVec.z = -1;
            CHECK(firstVec != secondVec);
        }
    }
    TEST_CASE("Negation - Mixed-sign values") {
        Vec3 firstVec{};
        firstVec.x = 2;
        firstVec.y = -2;
        firstVec.z = -1;
        firstVec = -firstVec;
        CHECK(firstVec.x == -2);
        CHECK(firstVec.y == 2);
        CHECK(firstVec.z == 1);
    }
    TEST_CASE("Addition - Mixed-sign values") {
        Vec3 firstVec{{2}, {-2}, {1}};
        Vec3 secondVec{{2}, {-2}, {-1}};

        Vec3 resultVec = firstVec + secondVec;
        CHECK(resultVec.x == 4);
        CHECK(resultVec.y == -4);
        CHECK(resultVec.z == 0);
    }
    TEST_CASE("Subtraction - Mixed-sign values") {
        Vec3 firstVec{{2}, {-2}, {1}};
        Vec3 secondVec{{2}, {-2}, {-1}};

        Vec3 resultVec = firstVec - secondVec;
        CHECK(resultVec.x == 0);
        CHECK(resultVec.y == 0);
        CHECK(resultVec.z == 2);
    }
}

TEST_SUITE("Vec4") {
    TEST_CASE("Equality - No initialized values") {
        Vec4 firstVec;
        Vec4 secondVec;

        CHECK(firstVec != secondVec);
    }
    TEST_CASE("Equality - Default initialized values") {
        Vec4 firstVec{};
        Vec4 secondVec{};

        CHECK(firstVec == secondVec);
    }
    TEST_CASE("Equality - Mixed-sign values") {
        Vec4 firstVec{{2}, {-2}, {2}, {-2}};
        Vec4 secondVec{{2}, {-2}, {2}, {-2}};

        CHECK(firstVec == secondVec);
    }
    TEST_CASE("Not Equal - Mixed-sign values") {
        Vec4 firstVec{};
        Vec4 secondVec{};

        SUBCASE("First value differs") {
            firstVec.x = -1;
            CHECK(firstVec != secondVec);
        }
        SUBCASE("Second value differs") {
            firstVec.y = -1;
            CHECK(firstVec != secondVec);
        }
        SUBCASE("Third value differs") {
            firstVec.z = -1;
            CHECK(firstVec != secondVec);
        }
        SUBCASE("Fourth value differs") {
            firstVec.w = -1;
            CHECK(firstVec != secondVec);
        }
    }
    TEST_CASE("Negation - Mixed-sign values") {
        Vec4 firstVec{};
        firstVec.x = 2;
        firstVec.y = -2;
        firstVec.z = -1;
        firstVec.w = 4;
        firstVec = -firstVec;
        CHECK(firstVec.x == -2);
        CHECK(firstVec.y == 2);
        CHECK(firstVec.z == 1);
        CHECK(firstVec.w == -4);
    }
    TEST_CASE("Addition - Mixed-sign values") {
        Vec4 firstVec{{2}, {-2}, {1}, {0}};
        Vec4 secondVec{{2}, {-2}, {-1}, {-2}};

        Vec4 resultVec = firstVec + secondVec;
        CHECK(resultVec.x == 4);
        CHECK(resultVec.y == -4);
        CHECK(resultVec.z == 0);
        CHECK(resultVec.w == -2);
    }
    TEST_CASE("Subtraction - Mixed-sign values") {
        Vec4 firstVec{{2}, {-2}, {1}, {0}};
        Vec4 secondVec{{2}, {-2}, {-1}, {-2}};

        Vec4 resultVec = firstVec - secondVec;
        CHECK(resultVec.x == 0);
        CHECK(resultVec.y == 0);
        CHECK(resultVec.z == 2);
        CHECK(resultVec.w == 2);
    }
}
