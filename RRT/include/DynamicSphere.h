#ifndef DYNAMICSPHERE_H
#define DYNAMICSPHERE_H

#include "Shape.h"
#include "Vector3.h"
#include "Ray.h"

class DynamicSphere : public Shape
{
public:
    DynamicSphere(const Vector3& center, float radius, const Rgb& color, float minTime, float maxTime);

    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const override;
    virtual bool shadowHit(const Ray& ray, float tmin, float tmax, float time) const override;

    const Vector3 getCenter(float time) const;

private:
    Vector3 _center;
    float _radius;
    Rgb _color;
    float _minTime;
    float _maxTime;
};

#endif // DYNAMICSPHERE_H
