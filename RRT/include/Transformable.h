#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "Matrix.h"
#include "Shape.h"

class Matrix;

class Transformable : public Shape {
public:
    Transformable(Shape* shape);
    virtual ~Transformable();

    void translate(const Vector3 &v);
    void translate(double x, double y, double z);
    void scale(const Vector3 &v);
    void scale(double x, double y, double z);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
    void rotate(double x, double y, double z);
    void rotateByPoint(double x, double y, double z, const Location &p);

    virtual bool hitTest(const Ray& ray, HitRecord& record) const override;

protected:
    Shape* _shape;
    Matrix _world, _invWorld;
};

#endif // TRANSFORMABLE_H
