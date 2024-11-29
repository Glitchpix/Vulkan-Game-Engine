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

Vec2 dot(const Vec2& lhs, const Vec2& rhs) {
    (void)rhs;
    return lhs;
}
Vec3 dot(const Vec3& lhs, const Vec3& rhs) {
    (void)rhs;
    return lhs;
}
Vec4 dot(const Vec4& lhs, const Vec4& rhs) {
    (void)rhs;
    return lhs;
}

Vec2 cross(const Vec2& lhs, const Vec2& rhs) {
    (void)rhs;
    return lhs;
}
Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
    (void)rhs;
    return lhs;
}
Vec4 cross(const Vec4& lhs, const Vec4& rhs) {
    (void)rhs;
    return lhs;
}