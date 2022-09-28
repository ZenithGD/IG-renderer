#include "coordinate.hpp"

ostream& operator<< (ostream& os, Coordinate coor){

    os << coor.matriz;

    return os;
}

Coordinate translation(const Vector3 v){

    Coordinate coor;

    coor.matriz[0][3] = v.x;
    coor.matriz[1][3] = v.y;
    coor.matriz[2][3] = v.z;

    return coor;
}

Coordinate rotationX(float theta) {

    Coordinate coor;

    float coseno = cos(theta);
    float seno = sin(theta);

    coor.matriz[1][1] = coseno;
    coor.matriz[2][1] = seno;
    coor.matriz[1][2] = -seno;
    coor.matriz[2][2] = coseno;

    return coor;
}

Coordinate rotationY( float theta){

    Coordinate coor;

    float coseno = cos(theta);
    float seno = sin(theta);

    coor.matriz[0][0] = coseno;
    coor.matriz[2][0] = -seno;
    coor.matriz[0][2] = seno;
    coor.matriz[2][2] = coseno;

    return coor;
}

Coordinate rotationZ(float theta){

    Coordinate coor;

    float coseno = cos(theta);
    float seno = sin(theta);
    
    coor.matriz[0][0] = coseno;
    coor.matriz[1][0] = seno;
    coor.matriz[0][1] = -seno;
    coor.matriz[1][1] = coseno;

    return coor;
}

Coordinate scale(const Vector3 v){

    Coordinate coor;

    coor.matriz[0][0] = v.x;
    coor.matriz[1][1] = v.y;
    coor.matriz[2][2] = v.z;

    return coor;
}

Coordinate changeBasis(const Vector3 u, const Vector3 v, const Vector3 w, const Vector3 o){

    Coordinate coor;

    coor.matriz[0][0] = u.x;
    coor.matriz[1][0] = u.y;
    coor.matriz[2][0] = u.z;

    coor.matriz[0][1] = v.x;
    coor.matriz[1][1] = v.y;
    coor.matriz[2][1] = v.z;

    coor.matriz[0][2] = w.x;
    coor.matriz[1][2] = w.y;
    coor.matriz[2][2] = w.z;

    coor.matriz[0][3] = o.x;
    coor.matriz[1][3] = o.y;
    coor.matriz[2][3] = o.z;

    return coor;
}

Coordinate inverseTransformation(Coordinate coor) {
    return Coordinate(coor.matriz.inverse());
}
