#include "mat4.hpp"

Mat4::Mat4(float m[4][4]) { 
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++){
            _mat[i][j] = m[i][j];
        }
    }
}

Mat4 Mat4::operator+(const Mat4 that) const {
    Mat4 result;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++){
            result[i][j] = _mat[i][j] + that[i][j];
        }
    }

    return result;
}

Mat4 Mat4::operator-(const Mat4 that) const {
    Mat4 result;
    
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            result[i][j] = _mat[i][j] - that[i][j];
        }
    }

    return result;
}

Mat4 Mat4::operator*(const Mat4 that) const{
    Mat4 result;
    for ( int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < 4; k++) {
                result[i][j] += _mat[i][k] * that[k][j]; 
            }
        }
    }

    return result;
}


float Mat4::det() const {
    // Adapted from https://stackoverflow.com/questions/2937702/i-want-to-find-determinant-of-4x4-matrix-in-c-sharp
    return
         _mat[0][3] * _mat[1][2] * _mat[2][1] * _mat[3][0] - _mat[0][2] * _mat[1][3] * _mat[2][1] * _mat[3][0] -
         _mat[0][3] * _mat[1][1] * _mat[2][2] * _mat[3][0] + _mat[0][1] * _mat[1][3] * _mat[2][2] * _mat[3][0] +
         _mat[0][2] * _mat[1][1] * _mat[2][3] * _mat[3][0] - _mat[0][1] * _mat[1][2] * _mat[2][3] * _mat[3][0] -
         _mat[0][3] * _mat[1][2] * _mat[2][0] * _mat[3][1] + _mat[0][2] * _mat[1][3] * _mat[2][0] * _mat[3][1] +
         _mat[0][3] * _mat[1][0] * _mat[2][2] * _mat[3][1] - _mat[0][0] * _mat[1][3] * _mat[2][2] * _mat[3][1] -
         _mat[0][2] * _mat[1][0] * _mat[2][3] * _mat[3][1] + _mat[0][0] * _mat[1][2] * _mat[2][3] * _mat[3][1] +
         _mat[0][3] * _mat[1][1] * _mat[2][0] * _mat[3][2] - _mat[0][1] * _mat[1][3] * _mat[2][0] * _mat[3][2] -
         _mat[0][3] * _mat[1][0] * _mat[2][1] * _mat[3][2] + _mat[0][0] * _mat[1][3] * _mat[2][1] * _mat[3][2] +
         _mat[0][1] * _mat[1][0] * _mat[2][3] * _mat[3][2] - _mat[0][0] * _mat[1][1] * _mat[2][3] * _mat[3][2] -
         _mat[0][2] * _mat[1][1] * _mat[2][0] * _mat[3][3] + _mat[0][1] * _mat[1][2] * _mat[2][0] * _mat[3][3] +
         _mat[0][2] * _mat[1][0] * _mat[2][1] * _mat[3][3] - _mat[0][0] * _mat[1][2] * _mat[2][1] * _mat[3][3] -
         _mat[0][1] * _mat[1][0] * _mat[2][2] * _mat[3][3] + _mat[0][0] * _mat[1][1] * _mat[2][2] * _mat[3][3];
}

Mat4 Mat4::inverse() const {
    // Adapted from https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
}

ostream& operator<<(ostream& os, const Mat4& mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            os << mat[i][j] << ", ";
        }
        os << mat[i][3] << endl;
    }
    return os;
}