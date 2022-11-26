#include <photonmapper/photon.hpp>

float Photon::position(std::size_t i) const{
    switch (i)
    {
    case 0:
        return position.x;
        break;
    case 1:
        return position.y;
        break;
    case 2:
        return position.z;
        break;
    default:
        throw runtime_error("Only 3 components for Vector3 !!");
        break;
    }
}