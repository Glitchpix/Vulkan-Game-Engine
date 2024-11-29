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
}
