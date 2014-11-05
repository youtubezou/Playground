#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere(const Vector3& center, double radius);
    virtual ~Sphere();

    const Vector3& c() const;
    double r() const;

    virtual bool hitTest(const Ray& ray, HitRecord& record) const override;

private:
    Vector3 _c;
    double _r;
};

inline const Vector3& Sphere::c() const
{
    return _c;
}

inline double Sphere::r() const
{
    return _r;
}

#endif // SPHERE_H
