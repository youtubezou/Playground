#include "Sphere.h"

Sphere::Sphere(const Vector3& center, float radius, const Rgb& color) :
    _center(center), _radius(radius), _color(color)
{
}

bool Sphere::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const
{
    (void)time;

    Vector3 o_c = ray.origin() - _center;

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
    record.normal = unitVector(ray.pointAt(t) - _center);
    record.color = _color;
    return true;
}

bool Sphere::shadowHit(const Ray& ray, float tmin, float tmax, float time) const
{
    (void)time;

    Vector3 o_c = ray.origin() - _center;

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
