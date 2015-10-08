#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "Vector3.h"

class Matrix {
public:
    constexpr Matrix();

    constexpr const double* operator[](int index);
    double* operator[](int index);

    static Matrix UNIT();
    friend Matrix operator+(const Matrix& a, const Matrix& b);
    friend Matrix operator-(const Matrix& a, const Matrix& b);
    friend Matrix operator*(const Matrix& a, const Matrix& b);
    friend Vector3 operator*(const Matrix& a, const Vector3& b);

    friend Matrix inverse(Matrix a);
    friend Matrix transpose(const Matrix& a);

    friend std::ostream& operator<<(std::ostream& out, const Matrix& mat);

private:
    double _data[4][4] = {{0.0}};
};

constexpr Matrix::Matrix() {
}

inline constexpr const double* Matrix::operator[](int index) {
    return _data[index];
}

inline double* Matrix::operator[](int index) {
    return _data[index];
}

inline Matrix Matrix::UNIT() {
    Matrix mat;
    for (int i = 0; i < 4; ++i) {
        mat[i][i] = 1.0;
    }
    return mat;
}

#endif // MATRIX_H
