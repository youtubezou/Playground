#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere(const Vector3& center, float radius, const Rgb& color);

    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const override;
    virtual bool shadowHit(const Ray& ray, float tmin, float tmax, float time) const override;

private:
    Vector3 _center;
    float _radius;
    Rgb _color;
};

#endif // SPHERE_H
