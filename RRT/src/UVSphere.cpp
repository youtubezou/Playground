#include "UVSphere.h"

UVSphere::UVSphere(const Vector3& center, float radius, Texture* texture) :
    _center(center), _radius(radius), _texture(texture)
{
}

bool UVSphere::hit(const Ray& ray, float tmin, float tmax, float, HitRecord& record) const
{
    Vector3 o_c = ray.origin() - _center;

    double A = dot(ray.direction(), ray.direction());
    double B = dot(2.0f * ray.direction(), o_c);
    double C = dot(o_c, o_c) - _radius*_radius;

    double discriminant = B*B - 4.0*A*C;
    if (discriminant < 0)
    {
        return false;
    }
    discriminant = sqrt(discriminant);
    double t = (- B - discriminant) / (2.0*A);
    if (t < tmin)
    {
        t = (- B + discriminant) / (2.0*A);
    }
    if (t < tmin || t > tmax)
    {
        return false;
    }
    record.t = t;
    record.point = ray.pointAt(t);
    record.normal = unitVector(record.point - _center);

    Vector3 n = record.normal;
    float twopi = 6.28318530718f;
    float theta = acos(n.z());
    float phi = atan2(n.y(), n.x());
    if (phi < 0.0f)
    {
        phi += twopi;
    }
    float oneOverPi = 3.18309886184f;
    float oneOver2Pi = oneOverPi * 2.0;
    record.uv = Vector2(phi/oneOver2Pi, (3.1415926535f - theta)/oneOverPi);
    record.texture = _texture;
    return true;
}

bool UVSphere::shadowHit(const Ray& ray, float tmin, float tmax, float) const
{
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

