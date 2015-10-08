#ifndef PARALLELOGRAM_H
#define PARALLELOGRAM_H

#include "Shape.h"
#include "Triangle.h"
#include "TriangleMesh.h"

class Parallelogram : public TriangleMesh {
public:
    Parallelogram(const Vector3& a = Vector3(), const Vector3& b = Vector3(), const Vector3& c = Vector3());
    virtual ~Parallelogram() = default;
};

#endif // PARALLELOGRAM_H
