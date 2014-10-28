#include "Triangle.h"

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c, const Rgb& color) :
    _a(a), _b(b), _c(c), _color(color)
{
}

bool Triangle::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const
{
    (void)time;

    float a = _a.x() - _b.x();
    float b = _a.y() - _b.y();
    float c = _a.z() - _b.z();

    float d = _a.x() - _c.x();
    float e = _a.y() - _c.y();
    float f = _a.z() - _c.z();

    float g = ray.direction().x();
    float h = ray.direction().y();
    float i = ray.direction().z();

    float j = _a.x() - ray.origin().x();
    float k = _a.y() - ray.origin().y();
    float l = _a.z() - ray.origin().z();

    float ei_hf = e*i - h*f;
    float gf_di = g*f - d*i;
    float dh_eg = d*h - e*g;

    float M = a*ei_hf + b*gf_di + c*dh_eg;

    float beta = (j*ei_hf + k*gf_di + l*dh_eg) / M;
    if (beta <= 0.0f || 1.0f <= beta)
    {
        return false;
    }

    float ak_jb = a*k - j*b;
    float jc_al = j*c - a*l;
    float bl_kc = b*l - k*c;

    float gamma = (i*ak_jb + h*jc_al + g*bl_kc) / M;
    if (gamma <= 0.0f || 1.0f <= beta + gamma)
    {
        return false;
    }

    float t = -(f*ak_jb + e*jc_al + d*bl_kc) / M;
    if (t < tmin || tmax < t)
    {
        return false;
    }
    record.t = t;
    record.normal = unitVector(cross(_b - _a, _c - _a));
    record.color = _color;
    return true;
}

bool Triangle::shadowHit(const Ray& ray, float tmin, float tmax, float time) const
{
    (void)time;

    float a = _a.x() - _b.x();
    float b = _a.y() - _b.y();
    float c = _a.z() - _b.z();

    float d = _a.x() - _c.x();
    float e = _a.y() - _c.y();
    float f = _a.z() - _c.z();

    float g = ray.direction().x();
    float h = ray.direction().y();
    float i = ray.direction().z();

    float j = _a.x() - ray.origin().x();
    float k = _a.y() - ray.origin().y();
    float l = _a.z() - ray.origin().z();

    float ei_hf = e*i - h*f;
    float gf_di = g*f - d*i;
    float dh_eg = d*h - e*g;

    float M = a*ei_hf + b*gf_di + c*dh_eg;

    float beta = (j*ei_hf + k*gf_di + l*dh_eg) / M;
    if (beta <= 0.0f || 1.0f <= beta)
    {
        return false;
    }

    float ak_jb = a*k - j*b;
    float jc_al = j*c - a*l;
    float bl_kc = b*l - k*c;

    float gamma = (i*ak_jb + h*jc_al + g*bl_kc) / M;
    if (gamma <= 0.0f || 1.0f <= beta + gamma)
    {
        return false;
    }

    float t = -(f*ak_jb + e*jc_al + d*bl_kc) / M;
    return tmin <= t && t <= tmax;
}
