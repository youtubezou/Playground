#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape
{
public:
    Triangle(const Vector3& a, const Vector3& b, const Vector3& c, const Rgb& color);

    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const override;
    virtual bool shadowHit(const Ray& ray, float tmin, float tmax, float time) const override;

private:
    Vector3 _a;
    Vector3 _b;
    Vector3 _c;
    Rgb _color;
};

#endif // TRIANGLE_H
