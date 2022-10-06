#include "mat4.hpp"

Mat4::Mat4(double m[4][4]) { 
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

Mat4 Mat4::operator*(const double f) const {
    Mat4 result;
    
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            result[i][j] = _mat[i][j] / f;
        }
    }

    return result;
}

Mat4 Mat4::operator/(const double f) const {
    Mat4 result;
    
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            result[i][j] = _mat[i][j] / f;
        }
    }

    return result;
}

double Mat4::det() const {
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

Mat4 Mat4::transpose() const {
    Mat4 tr;

    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            tr[i][j] = _mat[j][i];
        }
    }

    return tr;
}

double adjDet3(const Mat4& mat, unsigned int r, unsigned int c) {

    double temp[3][3];

    unsigned int ti = 0, tj = 0;
    for ( unsigned int i = 0; i < 4; i++ ) {
        for ( unsigned int j = 0; j < 4; j++ ) {
            if ( i != r && j != c ) {

                temp[ti][tj] = mat[i][j]; 
                tj++;
                if ( tj == 3 ) {
                    tj = 0;
                    ti++;
                }
            }
        }
    }

    return temp[0][0] * (temp[1][1] * temp[2][2] - temp[2][1] * temp[1][2])
         - temp[0][1] * (temp[1][0] * temp[2][2] - temp[2][0] * temp[1][2])
         + temp[0][2] * (temp[1][0] * temp[2][1] - temp[2][0] * temp[1][1]);
}

Mat4 Mat4::inverse() const {

    Mat4 adjMatrixT;

    int sign = 1;
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            adjMatrixT[i][j] = sign * adjDet3(*this, i, j);
            sign *= -1;
        }
        // (-1)^(i + j)
        sign *= -1;
    }

    return adjMatrixT.transpose() / det();
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