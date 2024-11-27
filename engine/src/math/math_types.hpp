#pragma once

#include "defines.hpp"

struct Vec2 {
    union {
        f32 x, r, s;
    };
    union {
        f32 y, g, t;
    };
};

struct Vec3 {
    union {
        f32 x, r, s;
    };
    union {
        f32 y, g, t;
    };
    union {
        f32 z, b, p;
    };
};

struct Vec4 {
    union {
        f32 x, r, s;
    };
    union {
        f32 y, g, t;
    };
    union {
        f32 z, b, p;
    };
    union {
        f32 w, a, q;
    };
};

using Quaternion = Vec4;