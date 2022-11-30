#pragma once

#include <math/vector3.hpp>
#include <list>
#include <acceleration/kdtree.hpp>

using namespace std;

class Photon {
public: 
    Vector3 position_;
    Vector3 inDirection;
    double flux;
    Vector3 normal;

    Photon(Vector3 pos, Vector3 idir, double f, Vector3 n) : position_(pos), inDirection(idir), flux(f), normal(n) {}
    double position(size_t i) const;
};

struct PhotonAxisPosition {
    double operator()(const Photon& p, size_t i) const {
        return p.position(i);
    }
};

