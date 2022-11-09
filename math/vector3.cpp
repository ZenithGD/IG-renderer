#include "vector3.hpp"
#include <cmath>

double Vector3::modulus() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() const {
    return *this / modulus();
}

Vector3 Vector3::projection(const Vector3 v) const {
    return v * (dot(v, *this) / dot(v, v));
}

double dot(const Vector3 u, const Vector3 v) {
    return u.x * v.x + u.y * v.y + u.z * v.z; 
}

Vector3 cross(const Vector3 u, const Vector3 v) {
    return Vector3( u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

Vector3 perpendicular(const Vector3 v) {
    if ( (v.z != 0) && (-v.x != v.y) ) {
        return Vector3(v.z, v.z, - v.x - v.y);
    } else {
        return Vector3(v.y - v.z, v.x, v.x);
    }
}

ostream& operator<<(ostream& os, const Vector3& v) {
    os << v.x << ", " << v.y << ", " << v.z;
    return os;
}