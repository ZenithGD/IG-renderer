#include <geometry/ray.hpp>

ostream& operator<<(ostream& os, const Ray& r) {
    os << "{ origin: " << r.origin << ", direction :" << r.direction << " }";
    return os;
}
