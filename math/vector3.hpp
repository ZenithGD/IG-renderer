#pragma once

#include <iostream>

using namespace std;

/**
 * @brief Class that represents a vector in 3d space
 */
struct Vector3 {
    // Vector components
    float x;
    float y;
    float z;

    /**
     * @brief Construct a new Vector 3 object
     * Null vector
     */
    inline Vector3() : x(0), y(0), z(0) {}

    /**
     * @brief Construct a new Vector 3 object
     * 
     * @param _x 
     * @param _y 
     * @param _z 
     */
    inline Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    /**
     * @brief Return the sum of two 3d vectors.
     * 
     * @param that The other vector 
     * @return Vector3 The sum vector 
     */
    inline Vector3 operator+(const Vector3 that) const { return Vector3(x + that.x, y + that.y, z + that.z); }

    /**
     * @brief Return the difference of two 3d vectors
     * 
     * @param that The other vector 
     * @return Vector3 The difference vector
     */
    inline Vector3 operator-(const Vector3 that) const { return Vector3(x - that.x, y - that.y, z - that.z);}

    /**
     * @brief Return a vector scaled by a scalar factor
     * 
     * @param that 
     * @return Vector3 
     */
    inline Vector3 operator*(const float that) const { return Vector3( x * that, y * that, z * that); }

    /**
     * @brief Return a vector dividde by a scalar factor
     * 
     * @param that 
     * @return Vector3 
     */
    inline Vector3 operator/(const float that) const { return Vector3( x / that, y / that, z / that); }

    /**
     * @brief Return the modulus of the Vector
     * 
     * @return float 
     */
    float modulus() const;

    /**
     * @brief Return the vector normalized
     * 
     * @return Vector3 
     */
    Vector3 normalized() const;

    /**
     * @brief Return the projection of this vector over v
     * 
     * @param v The projection direction vector
     * @return Vector3 
     */
    Vector3 projection(const Vector3 v) const;

    friend ostream& operator<<(const Vector3& v, ostream& os);
};

/**
 * @brief Show on the output stream the Vector
 *
 * @param os The output stream
 * @param v The vector that we want to see
 * @return ostream& 
 */
ostream& operator<<(ostream& os, const Vector3& v);

/**
 * @brief Returns the dot product of u and v
 * 
 * @param u One vector
 * @param v Another vector
 * @return float The dot product
 */
float dot(const Vector3 u, const Vector3 v);

/**
 * @brief Return the cross product of u and v
 * 
 * @param u One vector
 * @param v Another vector
 * @return Vector3 
 */
Vector3 cross(const Vector3 u, const Vector3 v);
