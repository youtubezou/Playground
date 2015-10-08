#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "Shape.h"
#include "Triangle.h"

class TriangleMesh : public Shape {
public:
    TriangleMesh(int num);
    virtual ~TriangleMesh();

    int num() const;
    Triangle& operator[](int index);
    const Triangle& operator[](int index) const;

    virtual bool hitTest(const Ray& ray, HitRecord& record) const override;

private:
    int _num;
    Triangle* _triangles;
};

inline int TriangleMesh::num() const {
    return _num;
}

inline Triangle& TriangleMesh::operator[](int index) {
    return _triangles[index];
}

inline const Triangle& TriangleMesh::operator[](int index) const {
    return _triangles[index];
}

#endif // TRIANGLEMESH_H
