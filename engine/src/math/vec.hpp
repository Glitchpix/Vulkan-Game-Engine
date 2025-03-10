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
constexpr bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
constexpr bool operator==(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
constexpr bool operator==(const Vec4& lhs, const Vec4& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}


constexpr Vec2 operator-(const Vec2& vec) {
    return Vec2{{-vec.x}, {-vec.y}};
}
constexpr Vec3 operator-(const Vec3& vec) {
    return Vec3{{-vec.x}, {-vec.y}, {-vec.z}};
}
constexpr Vec4 operator-(const Vec4& vec) {
    return Vec4{{-vec.x}, {-vec.y}, {-vec.z}, {-vec.w}};
}


constexpr Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2{{lhs.x + rhs.x}, {lhs.y + rhs.y}};
}
constexpr Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return Vec3{{lhs.x + rhs.x}, {lhs.y + rhs.y}, {lhs.z + rhs.z}};
}
constexpr Vec4 operator+(const Vec4& lhs, const Vec4& rhs) {
    return Vec4{{lhs.x + rhs.x}, {lhs.y + rhs.y}, {lhs.z + rhs.z}, {lhs.w + rhs.w}};
}

constexpr Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
    return Vec2{{lhs.x - rhs.x}, {lhs.y - rhs.y}};
}
constexpr Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
    return Vec3{{lhs.x - rhs.x}, {lhs.y - rhs.y}, {lhs.z - rhs.z}};
}
constexpr Vec4 operator-(const Vec4& lhs, const Vec4& rhs) {
    return Vec4{{lhs.x - rhs.x}, {lhs.y - rhs.y}, {lhs.z - rhs.z}, {lhs.w - rhs.w}};
}


constexpr f32 dot(const Vec2& lhs, const Vec2& rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}
constexpr f32 dot(const Vec3& lhs, const Vec3& rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}
constexpr f32 dot(const Vec4& lhs, const Vec4& rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w);
}


constexpr Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
    return Vec3{{lhs.y * rhs.z - lhs.z * rhs.y}, {lhs.z * rhs.x - lhs.x * rhs.z}, {lhs.x * rhs.y - lhs.y * rhs.x}};
}