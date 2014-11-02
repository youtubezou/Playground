#ifndef SHAPE_H
#define SHAPE_H

#include "Vector2.h"
#include "Vector3.h"
#include "Rgb.h"
#include "Ray.h"
#include "Texture.h"

struct HitRecord
{
    float t;
    Vector3 normal;
    Vector2 uv;
    Vector3 point;
    Texture* texture;

    HitRecord() :
        t(0.0f), normal(), uv(), point(), texture(nullptr)
    {
    }
};

class Shape
{
public:
    virtual ~Shape() = default;
    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const = 0;
    virtual bool shadowHit(const Ray& ray, float tmin, float tmax, float time) const = 0;
};

#endif // SHAPE_H
