#include "Transformable.h"

Transformable::Transformable(Shape* shape) :
    _shape(shape), _world(), _invWorld() {
    _world = Matrix::UNIT();
    _invWorld = Matrix::UNIT();
}

Transformable::~Transformable() {
    delete _shape;
}

void Transformable::translate(const Vector3 &v) {
    this->translate(v.x(), v.y(), v.z());
}

void Transformable::translate(double x, double y, double z) {
    Matrix transMat = Matrix::UNIT();
    transMat[0][3] = x;
    transMat[1][3] = y;
    transMat[2][3] = z;
    _world = transMat * _world;
    _invWorld = inverse(_world);
}

void Transformable::scale(const Vector3 &v) {
    this->scale(v.x(), v.y(), v.z());
}

void Transformable::scale(double x, double y, double z) {
    Matrix scaleMat = Matrix::UNIT();
    scaleMat[0][0] = x;
    scaleMat[1][1] = y;
    scaleMat[2][2] = z;
    _world = scaleMat * _world;
    _invWorld = inverse(_world);
}

void Transformable::rotateX(double angle) {
    Matrix rotateMat = Matrix::UNIT();
    rotateMat[1][1] = cos(angle);
    rotateMat[1][2] = -sin(angle);
    rotateMat[2][1] = sin(angle);
    rotateMat[2][2] = cos(angle);
    _world = rotateMat * _world;
    _invWorld = inverse(_world);
}

void Transformable::rotateY(double angle) {
    Matrix rotateMat = Matrix::UNIT();
    rotateMat[0][0] = cos(angle);
    rotateMat[0][2] = sin(angle);
    rotateMat[2][0] = -sin(angle);
    rotateMat[2][2] = cos(angle);
    _world = rotateMat * _world;
    _invWorld = inverse(_world);
}

void Transformable::rotateZ(double angle) {
    Matrix rotateMat = Matrix::UNIT();
    rotateMat[0][0] = cos(angle);
    rotateMat[0][1] = -sin(angle);
    rotateMat[1][0] = sin(angle);
    rotateMat[1][1] = cos(angle);
    _world = rotateMat * _world;
    _invWorld = inverse(_world);
}

void Transformable::rotate(double x, double y, double z) {
    this->rotateX(x);
    this->rotateY(y);
    this->rotateZ(z);
}

void Transformable::rotateByPoint(double x, double y, double z, const Location &p) {
    this->translate(-p);
    this->rotate(x, y, z);
    this->translate(p);
}

bool Transformable::hitTest(const Ray& ray, HitRecord& record) const {
    Vector3 no = _invWorld * ray.o();
    Vector3 nd = _invWorld * ray.d();
    for (int i = 0; i < 3; ++i) {
        nd[i] -= _invWorld[i][3];
    }
    Ray newRay(no, nd);
    if (_shape->hitTest(newRay, record)) {
        record.point = _world * record.point;
        record.normal = transpose(_invWorld) * record.normal;
        record.texture = texture();
        return true;
    }
    return false;
}
