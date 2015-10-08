#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(int num) :
    _num(num), _triangles(nullptr) {
    _triangles = new Triangle[num];
}

TriangleMesh::~TriangleMesh() {
    delete[] _triangles;
}

bool TriangleMesh::hitTest(const Ray& ray, HitRecord& record) const {
    HitRecord temp;
    record.t = 1e100;
    bool flag = false;
    for (int i = 0; i < _num; ++i) {
        if (_triangles[i].hitTest(ray, temp)) {
            if (temp.t < record.t) {
                record = temp;
            }
            flag = true;
        }
    }
    if (flag) {
        record.texture = texture();
    }
    return flag;
}
