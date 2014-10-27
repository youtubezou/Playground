#include <algorithm>
#include <iomanip>
#include "Matrix.h"
using namespace std;

Matrix operator+(const Matrix& a, const Matrix& b) {
    Matrix c;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}

Matrix operator-(const Matrix& a, const Matrix& b) {
    Matrix c;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    return c;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    Matrix c;
    for (int k = 0; k < 4; ++k) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

Vector3 operator*(const Matrix& a, const Vector3& b) {
    Vector3 c;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            c[i] += a[i][j] * b[j];
        }
        c[i] += a[i][3];
    }
    return c;
}

Matrix inverse(Matrix a) {
    Matrix inv = Matrix::UNIT();
    for (int i = 0; i < 4; ++i) {
        int index = i;
        for (int j = i + 1; j < 4; ++j) {
            if (fabs(a[j][i]) > fabs(a[index][i])) {
                index = j;
            }
        }
        if (index != i) {
            for (int j = 0; j < 4; ++j) {
                swap(a[i][j], a[index][j]);
                swap(inv[i][j], inv[index][j]);
            }
        }
        for (int j = i + 1; j < 4; ++j) {
            double scale = a[j][i] / a[i][i];
            for (int k = 0; k < 4; ++k) {
                a[j][k] -= a[i][k] * scale;
                inv[j][k] -= inv[i][k] * scale;
            }
        }
    }
    for (int i = 3; i > 0; --i) {
        for (int j = i - 1; j >= 0; --j) {
            double scale = a[j][i] / a[i][i];
            for (int k = 0; k < 4; ++k) {
                a[j][k] -= a[i][k] * scale;
                inv[j][k] -= inv[i][k] * scale;
            }
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            inv[i][j] /= a[i][i];
        }
    }
    return inv;
}

Matrix transpose(const Matrix& a) {
    Matrix b;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            b[i][j] = a[j][i];
        }
    }
    return b;
}

ostream& operator<<(ostream& out, const Matrix& mat) {
    ios init(nullptr);
    init.copyfmt(out);
    out << setiosflags(ios::fixed) << setiosflags(ios::right);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            out << setprecision(3) << mat[i][j] << '\t';
        }
        out << endl;
    }
    out << endl;
    out.copyfmt(init);
    return out;
}
