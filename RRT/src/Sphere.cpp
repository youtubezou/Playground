#include <cmath>
#include "Sphere.h"
const float PI = acos(-1.0);

Sphere::Sphere(const Vector3& center, float radius) :
    _c(center), _r(radius)
{
}

Sphere::~Sphere()
{
}

bool Sphere::hitTest(const Ray& ray, HitRecord& record) const
{
    Vector3 o_c = ray.o() - _c;
    float a = dot(ray.d(), ray.d());
    float b = dot(2.0f * ray.d(), o_c);
    float c = dot(o_c, o_c) - _r * _r;;
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
    {
        return false;
    }
    discriminant = sqrt(discriminant);
    float t = (- b - discriminant) / (2.0f * a);
    if (t < 0.0f)
    {
        t = (- b + discriminant) / (2.0f * a);
    }
    if (t < 0.0f)
    {
        return false;
    }
    record.t = t;
    record.point = ray.at(t);
    record.normal = (record.point - _c).norm();
    float phi = atan2(record.normal.x(), record.normal.z());
    if (phi < 0.0f)
    {
        phi += PI * 2.0f;
    }
    float theta = acos(record.normal.y());
    record.uv = Vector2(phi / (PI * 2.0f), theta / PI);
    record.texture = texture();
    return true;
}
