#include "Parallelogram.h"

Parallelogram::Parallelogram(const Vector3& a, const Vector3& b, const Vector3& c) :
    TriangleMesh(2) {
    (*this)[0] = Triangle(a, b, c);
    (*this)[1] = Triangle(b + c - a, c, b);
}
