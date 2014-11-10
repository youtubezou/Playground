#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "Matrix.h"
#include "Shape.h"

class Matrix;

class Transformable : public Shape
{
public:
    Transformable(Shape* shape);
    virtual ~Transformable();

    void translate(double x, double y, double z);
    void scale(double x, double y, double z);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);

    virtual bool hitTest(const Ray& ray, HitRecord& record) const override;

protected:
    Shape* _shape;
    Matrix _world, _invWorld;
};

#endif // TRANSFORMABLE_H
