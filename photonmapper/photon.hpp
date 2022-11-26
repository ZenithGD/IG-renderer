#pragma once

#include <math/vector3.hpp>

using namespace std;

class Photon {
public: 
    Vector3 position;
    Vector3 inDirection;
    double flux;
    Vector3 normal;

    Photon(Vector3 pos, Vector3 idir, double f, Vector3 n) : position(pos), inDirection(idir), flux(f), normal(n) {}
    float position(size_t i) const;
};

struct PhotonAxisPosition {
    float operator()(const Photon& p, size_t i) const {
        return p.position(i);
    }
};