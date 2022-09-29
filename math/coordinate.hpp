#pragma once

#include <cmath>
#include "vector3.hpp"
#include "mat4.hpp"

class Coordinate {

public:

    Mat4 matriz;

    Coordinate() : matriz() {}

    Coordinate(Vector3 u, Vector3 v, Vector3 w, Vector3 o) {

        matriz[0][0] = u.x;
        matriz[1][0] = u.y;
        matriz[2][0] = u.z;

        matriz[0][1] = v.x;
        matriz[1][1] = v.y;
        matriz[2][1] = v.z;

        matriz[0][2] = w.x;
        matriz[1][2] = w.y;
        matriz[2][2] = w.z;

        matriz[0][3] = o.x;
        matriz[1][3] = o.y;
        matriz[2][3] = o.z;

        matriz[3][3] = 1;
        
    }

    inline Coordinate(const Mat4 mat) : matriz(mat) {}

    Coordinate operator()(const Coordinate transform) { return Coordinate(matriz * transform.matriz); }

    friend ostream& operator<<(ostream& os, Coordinate coor);
};
ostream& operator<<(ostream& os, Coordinate coor);

Coordinate translation(const Vector3 v);

Coordinate rotationX(float theta);

Coordinate rotationY(float theta);

Coordinate rotationZ(float theta);

Coordinate scale(const Vector3 v);

Coordinate changeBasis(const Vector3 u, const Vector3 v, const Vector3 w, const Vector3 o);

Coordinate inverseTransformation(Coordinate coor);