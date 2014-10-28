#ifndef SHAPE_H
#define SHAPE_H

#include "Rgb.h"
#include "Ray.h"

struct HitRecord
{
    float t;
    Vector3 normal;
    Rgb color;
};

class Shape
{
public:
    virtual ~Shape() = default;
    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const = 0;
    virtual bool shadowHit(const Ray& ray, float tmin, float tmax, float time) const = 0;
};

#endif // SHAPE_H
