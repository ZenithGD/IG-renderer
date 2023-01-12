#include <geometry/ray.hpp>

ostream& operator<<(ostream& os, const Ray& r) {
    os << "{ origin: " << r.origin << ", direction :" << r.direction << " }";
    return os;
}

Vector3 setFaceNormal(const Vector3& rdir, const Vector3& n) {
    return dot(rdir, n) > 0 ? -n : n; 
}