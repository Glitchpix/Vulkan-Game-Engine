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

// Operators
bool operator==(const Vec2& lhs, const Vec2& rhs);
bool operator==(const Vec3& lhs, const Vec3& rhs);
bool operator==(const Vec4& lhs, const Vec4& rhs);

Vec2 operator-(const Vec2& vec);
Vec3 operator-(const Vec3& vec);
Vec4 operator-(const Vec4& vec);

Vec2 operator+(const Vec2& lhs, const Vec2& rhs);
Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
Vec4 operator+(const Vec4& lhs, const Vec4& rhs);

Vec2 operator-(const Vec2& lhs, const Vec2& rhs);
Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
Vec4 operator-(const Vec4& lhs, const Vec4& rhs);

Vec2 dot(const Vec2& lhs, const Vec2& rhs);
Vec3 dot(const Vec3& lhs, const Vec3& rhs);
Vec4 dot(const Vec4& lhs, const Vec4& rhs);

Vec2 cross(const Vec2& lhs, const Vec2& rhs);
Vec3 cross(const Vec3& lhs, const Vec3& rhs);
Vec4 cross(const Vec4& lhs, const Vec4& rhs);