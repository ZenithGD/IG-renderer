#include "coordinate.hpp"

ostream& operator<< (ostream& os, Coordinate coor){

    os << coor.matriz;

    return os;
}

Coordinate translation(const Coordinate coord, const Vector3 v){

    Coordinate transform;

    transform.matriz[0][3] = v.x;
    transform.matriz[1][3] = v.y;
    transform.matriz[2][3] = v.z;

    return transform(coord);
}

Coordinate rotationX(const Coordinate coord, float theta) {

    Coordinate transform;

    float coseno = cos(theta);
    float seno = sin(theta);

    transform.matriz[1][1] = coseno;
    transform.matriz[2][1] = seno;
    transform.matriz[1][2] = -seno;
    transform.matriz[2][2] = coseno;

    return transform(coord);
}

Coordinate rotationY(const Coordinate coord,  float theta){

    Coordinate transform;

    float coseno = cos(theta);
    float seno = sin(theta);

    transform.matriz[0][0] = coseno;
    transform.matriz[2][0] = -seno;
    transform.matriz[0][2] = seno;
    transform.matriz[2][2] = coseno;

    return transform(coord);
}

Coordinate rotationZ(const Coordinate coord, float theta){

    Coordinate transform;

    float coseno = cos(theta);
    float seno = sin(theta);
    
    transform.matriz[0][0] = coseno;
    transform.matriz[1][0] = seno;
    transform.matriz[0][1] = -seno;
    transform.matriz[1][1] = coseno;

    return transform(coord);
}

Coordinate scale(const Coordinate coord, const Vector3 v){

    Coordinate transform;

    transform.matriz[0][0] = v.x;
    transform.matriz[1][1] = v.y;
    transform.matriz[2][2] = v.z;

    return transform(coord);
}

Coordinate changeBasis(const Coordinate coord, const Vector3 u, const Vector3 v, const Vector3 w, const Vector3 o){

    Coordinate transform;

    transform.matriz[0][0] = u.x;
    transform.matriz[1][0] = u.y;
    transform.matriz[2][0] = u.z;

    transform.matriz[0][1] = v.x;
    transform.matriz[1][1] = v.y;
    transform.matriz[2][1] = v.z;

    transform.matriz[0][2] = w.x;
    transform.matriz[1][2] = w.y;
    transform.matriz[2][2] = w.z;

    transform.matriz[0][3] = o.x;
    transform.matriz[1][3] = o.y;
    transform.matriz[2][3] = o.z;

    return transform(coord);
}

Coordinate inverseTransformation(Coordinate coor) {
    return Coordinate(coor.matriz.inverse());
}
