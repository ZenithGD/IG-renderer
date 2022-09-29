#pragma once

#include <cmath>
#include "vector3.hpp"
#include "mat4.hpp"
/**
 * @brief Class for representing homogenous coordinates
 * 
 */
class Coordinate {

public:

    Mat4 matrix;

    /**
     * @brief Construct a new empty Coordinate object
     * This will create an identity matrix
     */
    Coordinate() : matrix() {}

    /**
     * @brief Construct a new Coordinate object
     * 
     * @param u The first axis vector of the coordinate system
     * @param v The first axis vector of the coordinate system
     * @param w The first axis vector of the coordinate system
     * @param o The origin of the coordinate system
     * @param wCoord The homogeneous coordinate
     */
    Coordinate(Vector3 u, Vector3 v, Vector3 w, Vector3 o, float wCoord) {

        matrix[0][0] = u.x;
        matrix[1][0] = u.y;
        matrix[2][0] = u.z;

        matrix[0][1] = v.x;
        matrix[1][1] = v.y;
        matrix[2][1] = v.z;

        matrix[0][2] = w.x;
        matrix[1][2] = w.y;
        matrix[2][2] = w.z;

        matrix[0][3] = o.x;
        matrix[1][3] = o.y;
        matrix[2][3] = o.z;

        matrix[3][3] = wCoord;
    }

    /**
     * @brief Construct a new Coordinate object with a 4x4 matrix.
     * 
     * @param mat The matrix object
     */
    inline Coordinate(const Mat4 mat) : matrix(mat) {}

    /**
     * @brief Get the origin of the coordinate system
     * 
     * @return Vector3 
     */
    inline Vector3 getPosition() const { return Vector3(matrix[0][3], matrix[1][3], matrix[2][3]); }

    /**
     * @brief Apply the transformation to coord
     * 
     * @param coord The coordinates to transform 
     * @return Coordinate 
     */
    Coordinate operator()(const Coordinate coord) { return Coordinate(matrix * coord.matrix); }

    friend ostream& operator<<(ostream& os, Coordinate coor);
};

/**
 * @brief Print coor on the output stream
 * 
 * @param os The output stream
 * @param coor The coordinate system
 * @return ostream& 
 */
ostream& operator<<(ostream& os, Coordinate coor);

Coordinate translation(const Coordinate coord, const Vector3 v);

Coordinate rotationX(const Coordinate coord, float theta);

Coordinate rotationY(const Coordinate coord, float theta);

Coordinate rotationZ(const Coordinate coord, float theta);

Coordinate scale(const Coordinate coord, const Vector3 v);

Coordinate changeBasis(const Coordinate coord, const Vector3 u, const Vector3 v, const Vector3 w, const Vector3 o);

Coordinate inverseTransformation(Coordinate coor);