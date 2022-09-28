#pragma once

#include <iostream>

using namespace std;

/**
 * @brief Class for representing 4x4 float matrices
 * 
 */
class Mat4 {
public:

    /**
     * @brief Construct a new Mat 4 object
     * By default, it creates a 4x4 identity matrix
     */
    inline Mat4() : _mat{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}} {};

    /**
     * @brief Construct a new Mat 4 object with values 
     * @param m The value 2d array
     */
    Mat4(float m[4][4]);

    /**
     * @brief Return the sum of two matrices
     * 
     * @param that The other matrix
     * @return Mat4 The sum matrix
     */
    Mat4 operator+(const Mat4 that) const;
    
    /**
     * @brief Return the difference of two matrices
     * 
     * @param that The other matrix
     * @return Mat4 The difference matrix
     */
    Mat4 operator-(const Mat4 that) const;

    /**
     * @brief Return the matrix product of two matrices
     * 
     * @param that The other matrix
     * @return Mat4 The product matrix
     */
    Mat4 operator*(const Mat4 that) const;

    /**
     * @brief Return the matrix multiplied by the scalar f
     * 
     * @param f the scalar value
     * @return Mat4 The scaled-up matrix
     */
    Mat4 operator*(const float f) const;

    /**
     * @brief Return the matrix divided by the scalar f
     * 
     * @param f the scalar value
     * @return Mat4 The scaled-down matrix
     */
    Mat4 operator/(const float f) const;

    /**
     * @brief Return the determinant of this matrix.
     * 
     * @return float The determinant
     */
    float det() const;

    /**
     * @brief Return the inverse of this matrix.
     * 
     * @return Mat4 
     */
    Mat4 inverse() const;

    /**
     * @brief Return the transpose of this matrix.
     * 
     * @return Mat4 The transposed matrix
     */
    Mat4 transpose() const;

    /**
     * @brief Return a matrix row by index
     * 
     * @param i The row index
     * @return decltype(auto) The row of the matrix 
     */
    decltype(auto) operator[](unsigned int i) { return _mat[i]; }
    decltype(auto) operator[](unsigned int i) const { return _mat[i]; }

    /**
     * @brief Print the matrix on an output stream
     * 
     * @param os The output stream
     * @param mat The matrix
     * @return ostream&
     */
    friend ostream& operator<<(ostream& os, const Mat4& mat);
private: 
    /**
     * @brief The inner representation of the 4x4 matrix.
     */
    float _mat[4][4];
};

ostream& operator<<(ostream& os, const Mat4& mat);

