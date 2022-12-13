#pragma once

#include <math/vector3.hpp>
#include <color/color.hpp>
#include <list>
#include <acceleration/kdtree.hpp>

using namespace std;

class Photon {
public: 
    Vector3 position_;
    Vector3 inDirection;
    RGB flux;
    Vector3 normal;

    Photon(const Vector3& pos, const Vector3& idir, const RGB& f, const Vector3& n) : position_(pos), inDirection(idir), flux(f), normal(n) {}
    double position(size_t i) const;
};

struct PhotonAxisPosition {
    double operator()(const Photon& p, size_t i) const {
        return p.position(i);
    }
};

