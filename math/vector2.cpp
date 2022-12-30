#include "vector2.hpp"

#include <cmath>
#include <core/core.hpp>

double Vector2::modulus() const {
    return sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const {
    return *this / modulus();
}

double dot(const Vector2 u, const Vector2 v) {
    return u.x * v.x + u.y * v.y; 
}

ostream& operator<<(ostream& os, const Vector2& v) {
    os << v.x << ", " << v.y;
    return os;
}