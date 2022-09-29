#include "coordinate.hpp"

ostream& operator<< (ostream& os, Coordinate coor){

    os << coor.matrix;

    return os;
}

Coordinate translation(const Coordinate coord, const Vector3 v){

    Coordinate transform;

    transform.matrix[0][3] = v.x;
    transform.matrix[1][3] = v.y;
    transform.matrix[2][3] = v.z;

    return transform(coord);
}

Coordinate rotationX(const Coordinate coord, float theta) {

    Coordinate transform;

    float coseno = cos(theta);
    float seno = sin(theta);

    transform.matrix[1][1] = coseno;
    transform.matrix[2][1] = seno;
    transform.matrix[1][2] = -seno;
    transform.matrix[2][2] = coseno;

    return transform(coord);
}

Coordinate rotationY(const Coordinate coord,  float theta){

    Coordinate transform;

    float coseno = cos(theta);
    float seno = sin(theta);

    transform.matrix[0][0] = coseno;
    transform.matrix[2][0] = -seno;
    transform.matrix[0][2] = seno;
    transform.matrix[2][2] = coseno;

    return transform(coord);
}

Coordinate rotationZ(const Coordinate coord, float theta){

    Coordinate transform;

    float coseno = cos(theta);
    float seno = sin(theta);
    
    transform.matrix[0][0] = coseno;
    transform.matrix[1][0] = seno;
    transform.matrix[0][1] = -seno;
    transform.matrix[1][1] = coseno;

    return transform(coord);
}

Coordinate scale(const Coordinate coord, const Vector3 v){

    Coordinate transform;

    transform.matrix[0][0] = v.x;
    transform.matrix[1][1] = v.y;
    transform.matrix[2][2] = v.z;

    return transform(coord);
}

Coordinate changeBasis(const Coordinate coord, const Vector3 u, const Vector3 v, const Vector3 w, const Vector3 o){

    Coordinate transform;

    transform.matrix[0][0] = u.x;
    transform.matrix[1][0] = u.y;
    transform.matrix[2][0] = u.z;

    transform.matrix[0][1] = v.x;
    transform.matrix[1][1] = v.y;
    transform.matrix[2][1] = v.z;

    transform.matrix[0][2] = w.x;
    transform.matrix[1][2] = w.y;
    transform.matrix[2][2] = w.z;

    transform.matrix[0][3] = o.x;
    transform.matrix[1][3] = o.y;
    transform.matrix[2][3] = o.z;

    return transform(coord);
}

Coordinate inverseTransformation(Coordinate coor) {
    return Coordinate(coor.matrix.inverse());
}
