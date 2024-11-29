#include "vec.hpp"

bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
bool operator==(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
bool operator==(const Vec4& lhs, const Vec4& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}


Vec2 operator-(const Vec2& vec) {
    return Vec2{{-vec.x}, {-vec.y}};
}
Vec3 operator-(const Vec3& vec) {
    return Vec3{{-vec.x}, {-vec.y}, {-vec.z}};
}
Vec4 operator-(const Vec4& vec) {
    return Vec4{{-vec.x}, {-vec.y}, {-vec.z}, {-vec.w}};
}

Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2{{lhs.x + rhs.x}, {lhs.y + rhs.y}};
}
Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return Vec3{{lhs.x + rhs.x}, {lhs.y + rhs.y}, {lhs.z + rhs.z}};
}
Vec4 operator+(const Vec4& lhs, const Vec4& rhs) {
    return Vec4{{lhs.x + rhs.x}, {lhs.y + rhs.y}, {lhs.z + rhs.z}, {lhs.w + rhs.w}};
}

Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
    return Vec2{{lhs.x - rhs.x}, {lhs.y - rhs.y}};
}
Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
    return Vec3{{lhs.x - rhs.x}, {lhs.y - rhs.y}, {lhs.z - rhs.z}};
}
Vec4 operator-(const Vec4& lhs, const Vec4& rhs) {
    return Vec4{{lhs.x - rhs.x}, {lhs.y - rhs.y}, {lhs.z - rhs.z}, {lhs.w - rhs.w}};
}

f32 dot(const Vec2& lhs, const Vec2& rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}
f32 dot(const Vec3& lhs, const Vec3& rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}
f32 dot(const Vec4& lhs, const Vec4& rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w);
}

Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
    return Vec3{{lhs.y * rhs.z - lhs.z * rhs.y}, {lhs.z * rhs.x - lhs.x * rhs.z}, {lhs.x * rhs.y - lhs.y * rhs.x}};
}