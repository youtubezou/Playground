#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere(const Vector3& center, float radius);
    virtual ~Sphere();

    virtual bool hitTest(const Ray& ray, HitRecord& record) const override;

private:
    Vector3 _c;
    float _r;
};

#endif // SPHERE_H
