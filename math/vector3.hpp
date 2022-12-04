#pragma once

#include <iostream>

using namespace std;

/**
 * @brief Class that represents a vector in 3d space
 */
struct Vector3 {
    // Vector components
    double x;
    double y;
    double z;

    /**
     * @brief Construct a new Vector 3 object
     * Default constructor initializes to zero vector
     */
    inline Vector3() : x(0), y(0), z(0) {}

    /**
     * @brief Construct a new Vector 3 object
     * 
     * @param _x The x coord
     * @param _y The y coord
     * @param _z The z coord
     */
    inline Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

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
    inline Vector3 operator*(const double that) const { return Vector3( x * that, y * that, z * that); }

    /**
     * @brief Return a vector divided by a scalar factor
     * 
     * @param that 
     * @return Vector3 
     */
    inline Vector3 operator/(const double that) const { return Vector3( x / that, y / that, z / that); }

    /**
     * @brief Get a vector with the same direction and modulus and opposite direction
     * 
     * @return Vector3 
     */
    inline Vector3 operator-() const { return Vector3( -x, -y, -z ); }

    /**
     * @brief Return the modulus of the Vector
     * 
     * @return double 
     */
    double modulus() const;

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

    const double& operator[](const unsigned int i) const {
        switch (i)
        {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        default:
            throw runtime_error("Only 3 components for Vector3 !!");
            break;
        }
    }

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
 * @return double The dot product
 */
double dot(const Vector3 u, const Vector3 v);

/**
 * @brief Return the cross product of u and v
 * 
 * @param u One vector
 * @param v Another vector
 * @return Vector3 
 */
Vector3 cross(const Vector3 u, const Vector3 v);

/**
 * @brief Return v as a normalized vector 
 * 
 * @param v 
 * @return Vector3 
 */
inline Vector3 normalize(const Vector3 v) { return v.normalized(); }

/**
 * @brief Find any perpendicular vector to v
 * 
 * @param v The vector
 * @return Vector3 The perpendicular vector
 */
Vector3 perpendicular(const Vector3 v);

/**
 * @brief Return delta function of a vector in one direction.
 * 
 * @param dir The direction to check
 * @param v The vector to check against the direction
 * @return double 1 if vectors have the same direction and orientation,
 * 0 otherwise
 */
double delta(Vector3 dir, Vector3 v);

/**
 * @brief Return the angle between dir1 and n
 * 
 * @param dir Direction of the ray
 * @param n Vector normal
 * @return double 
 */
double angle(Vector3 dir, Vector3 n);

inline double modulus(const Vector3& v) { return v.modulus(); }