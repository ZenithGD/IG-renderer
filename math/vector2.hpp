#pragma once

#include <iostream>

using namespace std;

/**
 * @brief Class that represents a vector in 2d space
 */
struct Vector2 {
    // Vector components
    double x;
    double y;

    /**
     * @brief Construct a new Vector 3 object
     * Default constructor initializes to zero vector
     */
    inline Vector2() : x(0), y(0) {}

    /**
     * @brief Construct a new Vector 2 object
     * 
     * @param _x The x coord
     * @param _y The y coord
     */
    inline Vector2(double _x, double _y) : x(_x), y(_y) {}

    /**
     * @brief Return the sum of two 3d vectors.
     * 
     * @param that The other vector 
     * @return Vector2 The sum vector 
     */
    inline Vector2 operator+(const Vector2 that) const { return Vector2(x + that.x, y + that.y); }

    /**
     * @brief Return the difference of two 3d vectors
     * 
     * @param that The other vector 
     * @return Vector2 The difference vector
     */
    inline Vector2 operator-(const Vector2 that) const { return Vector2(x - that.x, y - that.y);}

    /**
     * @brief Return a vector scaled by a scalar factor
     * 
     * @param that 
     * @return Vector2 
     */
    inline Vector2 operator*(const double that) const { return Vector2( x * that, y * that); }

    /**
     * @brief Return a vector divided by a scalar factor
     * 
     * @param that 
     * @return Vector2 
     */
    inline Vector2 operator/(const double that) const { return Vector2( x / that, y / that); }

    /**
     * @brief Get a vector with the same direction and modulus and opposite direction
     * 
     * @return Vector2 
     */
    inline Vector2 operator-() const { return Vector2( -x, -y ); }

    /**
     * @brief Return the modulus of the Vector
     * 
     * @return double 
     */
    double modulus() const;

    /**
     * @brief Return the vector normalized
     * 
     * @return Vector2 
     */
    Vector2 normalized() const;

    friend ostream& operator<<(ostream& os, const Vector2& v);
};

/**
 * @brief Show on the output stream the Vector
 *
 * @param os The output stream
 * @param v The vector that we want to see
 * @return ostream& 
 */
ostream& operator<<(ostream& os, const Vector2& v);

double dot(const Vector2 u, const Vector2 v);

inline double modulus(const Vector2& v) { return v.modulus(); }
