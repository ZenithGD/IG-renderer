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
     * @brief Construct a new empty Coordinate object
     * @param wCord Value of w
     * This will create an identity matrix
     */
    Coordinate(double wCoord) : matrix() { matrix[3][3] = wCoord; }

    /**
     * @brief Construct a new Coordinate object
     * 
     * @param u The first axis vector of the coordinate system
     * @param v The first axis vector of the coordinate system
     * @param w The first axis vector of the coordinate system
     * @param o The origin of the coordinate system
     * @param wCoord The homogeneous coordinate
     */
    Coordinate(Vector3 u, Vector3 v, Vector3 w, Vector3 o = Vector3(), double wCoord = 1) {

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

/**
 * @brief Translates coord by a translation vector v.
 * 
 * @param coord The coordinate object
 * @param v The translation vector
 * @return Coordinate 
 */
Coordinate translation(const Coordinate coord, const Vector3 v);

/**
 * @brief Rotate the coordinate system by theta radians on the X axis.
 * 
 * @param coord The coordinate object
 * @param theta The rotation angle, in radians 
 * @return Coordinate 
 */
Coordinate rotationX(const Coordinate coord, double theta);

/**
 * @brief Rotate the coordinate system by theta radians on the Y axis.
 * 
 * @param coord The coordinate object
 * @param theta The rotation angle, in radians 
 * @return Coordinate 
 */
Coordinate rotationY(const Coordinate coord, double theta);

/**
 * @brief Rotate the coordinate system by theta radians on the Z axis.
 * 
 * @param coord The coordinate object
 * @param theta The rotation angle, in radians 
 * @return Coordinate 
 */
Coordinate rotationZ(const Coordinate coord, double theta);

/**
 * @brief Scale the coordinate system by a scale vector
 * This will scale by v.x on the X axis, v.y on the Y axis, and v.z on the Z axis
 * @param coord The coordinate object
 * @param v The scale vector
 * @return Coordinate 
 */
Coordinate scale(const Coordinate coord, const Vector3 v);

/**
 * @brief Change the coordinate basis.
 * This will apply a change of basis from the new one to the current coordinate system
 * @param coord The coordinate object
 * @param u The new basis's first axis in the old basis's coordinates
 * @param v The new basis's second axis in the old basis's coordinates
 * @param w The new basis's third axis in the old basis's coordinates
 * @param o The new basis's origin in the old basis's coordinates
 * @return Coordinate 
 */
Coordinate changeBasis(const Coordinate coord, const Vector3 u, const Vector3 v, const Vector3 w, const Vector3 o);

/**
 * @brief Return the inverse transform of the coordinate object.
 * Applying the inverse transformation to the coordinate object should return
 * an identity coordinate object.
 * @param coor The coordinate object
 * @return Coordinate 
 */
Coordinate inverseTransformation(Coordinate coor);