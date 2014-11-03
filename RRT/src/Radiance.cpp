#include <cstdlib>
#include "Radiance.h"
const float PI = acos(-1.0);

inline float random()
{
    return (float)rand() / (float)RAND_MAX;
}

Radiance::Radiance(Shape** shapes, int n, const Color& background) :
    _shapes(shapes), _shapeNum(n), _background(background)
{
}

Radiance::~Radiance()
{
}

Color Radiance::radiance(const Ray& ray, int depth)
{
    Shape* shape = nullptr;
    HitRecord record, tempRecord;
    record.t = 1e10f;
    for (int i = 0; i < _shapeNum; ++i)
    {
        if (_shapes[i]->hitTest(ray, tempRecord))
        {
            if (tempRecord.t > 1e-4f && tempRecord.t < record.t)
            {
                record = tempRecord;
                shape = _shapes[i];
            }
        }
    }
    if (shape == nullptr)
    {
        return _background;
    }
    if (depth >= 5)
    {
        return shape->emission();
    }
    switch (shape->material())
    {
    case Material::DIFFUSE:
        return idealDiffuse(ray, record, shape, depth);
    case Material::SPECULAR:
        return idealSpecular(ray, record, shape, depth);
    case Material::REFRACTION:
        return idealRefraction(ray, record, shape, depth);
    }
    return _background;
}

Color Radiance::idealDiffuse(const Ray& ray, const HitRecord& record, Shape* shape, int depth)
{
    float r1 = random();
    float r2 = random();
    float costheta = sqrt(1.0f - r1);
    float sintheta = sqrt(1.0f - costheta * costheta);
    float phi = 2.0f * PI * r2;
    Vector3 w = record.normal;
    if (dot(ray.d(), record.normal) > 0.0f)
    {
        w = -w;
    }
    Vector3 u;
    if (fabs(w.x()) < 0.01f)
    {
        u = cross(Vector3(1.0f, 0.0f, 0.0f), w).norm();
    }
    else
    {
        u = cross(Vector3(0.0f, 1.0f, 0.0f), w).norm();
    }
    Vector3 v = cross(w, u);
    Vector3 b = cos(phi) * sintheta * u +
                sin(phi) * sintheta * v +
                costheta * w;
    Ray r(record.point, b);
    Color color = record.texture->getColor(record.uv);
    return shape->emission() + color * radiance(r, depth + 1);
}

Color Radiance::idealSpecular(const Ray& ray, const HitRecord& record, Shape* shape, int depth)
{
    Vector3 d = ray.d() - 2.0f * dot(ray.d(), record.normal) * record.normal;
    Ray r(record.point, d);
    return shape->emission() + record.texture->getColor(record.uv) * radiance(r, depth + 1);
}

Color Radiance::idealRefraction(const Ray& ray, const HitRecord& record, Shape* shape, int depth)
{
    Vector3 d = ray.d() - 2.0f * dot(ray.d(), record.normal) * record.normal;
    Ray reflect(record.point, d);
    bool in = dot(ray.d(), record.normal) < 0.0f;
    float nc = 1.0f;
    float nt = shape->dielectric();
    float refraction = nc / nt;
    if (!in)
    {
        refraction = nt / nc;
    }
    float ddn = in ? dot(ray.d(), record.normal) : dot(ray.d(), -record.normal);
    float cos2t = 1.0f - refraction * refraction * (1.0f - ddn * ddn);
    if (cos2t < 0.0f)
    {
        return shape->emission() + record.texture->getColor(record.uv) * radiance(reflect, depth + 1);
    }
    Vector3 td = (ray.d() * refraction - record.normal * ((in ? 1 : -1) * (refraction * ddn + sqrt(cos2t)))).norm();
    float a = nt - nc;
    float b = nt + nc;
    float R0 = a * a / (b * b);
    float c = 1.0f - (in ? -ddn : dot(td, record.normal));
    float Re = R0 + (1 - R0) * c * c * c * c * c;
    float Tr = 1 - Re;
    float P = 0.25f + 0.5f * Re;
    float RP = Re / P;
    float TP = Tr / (1.0f - P);
    if (depth >= 2)
    {
        if (random() < P)
        {
            return shape->emission() + record.texture->getColor(record.uv) * radiance(reflect, depth + 1) * RP;
        }
        return shape->emission() + record.texture->getColor(record.uv) * radiance(Ray(record.point, td), depth + 1) * TP;
    }
    return shape->emission() + record.texture->getColor(record.uv) * radiance(reflect, depth + 1) * Re +
                               record.texture->getColor(record.uv) * radiance(Ray(record.point, td), depth + 1) * Tr;
}
