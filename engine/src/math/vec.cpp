#include "vec.hpp"

bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
bool operator==(const Vec3& lhs, const Vec3& rhs) {
    (void)lhs;
    (void)rhs;
    return false;
}
bool operator==(const Vec4& lhs, const Vec4& rhs) {
    (void)lhs;
    (void)rhs;
    return false;
}
