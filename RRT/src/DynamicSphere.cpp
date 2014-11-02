#include "DynamicSphere.h"

DynamicSphere::DynamicSphere(const Vector3& center, float radius, const Rgb& color, float minTime, float maxTime) :
    _center(center), _radius(radius), _color(color), _minTime(minTime), _maxTime(maxTime)
{
}

bool DynamicSphere::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const
{
    Vector3 newCenter = getCenter(time);

    Vector3 o_c = ray.origin() - newCenter;

    double A = dot(ray.direction(), ray.direction());
    double B = dot(2.0f * ray.direction(), o_c);
    double C = dot(o_c, o_c) - _radius*_radius;

    double discriminant = B*B - 4*A*C;
    if (discriminant < 0)
    {
        return false;
    }
    discriminant = sqrt(discriminant);
    double t = (- B - discriminant) / (2.0f*A);
    if (t < tmin)
    {
        t = (- B + discriminant) / (2.0f*A);
    }
    if (t < tmin || t > tmax)
    {
        return false;
    }
    record.t = t;
    record.normal = unitVector(ray.pointAt(t) - newCenter);
    //record.color = _color;
    return true;
}

bool DynamicSphere::shadowHit(const Ray& ray, float tmin, float tmax, float time) const
{
    Vector3 newCenter = getCenter(time);

    Vector3 o_c = ray.origin() - newCenter;

    double A = dot(ray.direction(), ray.direction());
    double B = dot(2.0f * ray.direction(), o_c);
    double C = dot(o_c, o_c) - _radius*_radius;

    double discriminant = B*B - 4*A*C;
    if (discriminant < 0)
    {
        return false;
    }
    discriminant = sqrt(discriminant);
    double t = (- B - discriminant) / (2.0f*A);
    if (t < tmin)
    {
        t = (- B + discriminant) / (2.0f*A);
    }
    if (t < tmin || t > tmax)
    {
        return false;
    }
    return true;
}

const Vector3 DynamicSphere::getCenter(float time) const
{
    float realtime = time * _maxTime + (1.0f - time) * _minTime;
    return Vector3(_center.x() + realtime,
                   _center.y() + realtime,
                   _center.z() + realtime);
}
