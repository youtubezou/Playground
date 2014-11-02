#ifndef UVSPHERE_H
#define UVSPHERE_H

#include "Shape.h"
#include "Vector3.h"
#include "Ray.h"
#include "Texture.h"

class UVSphere : public Shape
{
public:
    UVSphere(const Vector3& center, float radius, Texture* texture);
    virtual ~UVSphere() noexcept(true) = default;
    UVSphere(const UVSphere&) = default;
    UVSphere& operator =(const UVSphere&) = default;

    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const override;
    virtual bool shadowHit(const Ray& ray, float tmin, float tmax, float time) const override;

private:
    Vector3 _center;
    float _radius;
    Texture* _texture;
};

#endif // UVSPHERE_H
