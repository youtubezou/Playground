#include <cstdlib>
#include <algorithm>
#include "Sphere.h"
#include "Radiance.h"
using namespace std;
const double PI = acos(-1.0);

inline double random()
{
    return (double)rand() / (double)RAND_MAX;
}

Radiance::Radiance(Shape** shapes, int n, const Color& background) :
    _shapes(shapes), _shapeNum(n), _background(background)
{
}

Radiance::~Radiance()
{
}

Color Radiance::radiance(const Ray& ray, int depth, bool emit)
{
    Shape* shape = nullptr;
    HitRecord record, tempRecord;
    record.t = 1e100;
    for (int i = 0; i < _shapeNum; ++i)
    {
        if (_shapes[i]->hitTest(ray, tempRecord))
        {
            if (tempRecord.t < record.t)
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
    Color color = record.texture->getColor(record.uv);
    if (fabs(color.r()) < 1e-6 && fabs(color.g()) < 1e-6 && fabs(color.b()) < 1e-6)
    {
        return shape->emission();
    }
    double p = max(max(color.r(), color.g()), color.b());
    if (depth >= 5)
    {
        if (random() > p)
        {
            return shape->emission();
        }
        color = color * (1.0 / p);
    }
    if (depth >= 100)
    {
        return shape->emission();
    }
    switch (shape->material())
    {
    case Material::DIFFUSE:
        return idealDiffuse(ray, record, shape, color, depth, emit);
    case Material::SPECULAR:
        return idealSpecular(ray, record, shape, color, depth);
    case Material::REFRACTION:
        return idealRefraction(ray, record, shape, color, depth);
    }
    return _background;
}

Color Radiance::idealDiffuse(const Ray& ray, const HitRecord& record, Shape* shape, const Color& color, int depth, bool emit)
{
    double r1 = random();
    double r2 = random();
    double costheta = sqrt(1.0 - r1);
    double sintheta = sqrt(1.0 - costheta * costheta);
    double phi = 2.0 * PI * r2;
    Vector3 w = record.normal;
    if (dot(ray.d(), record.normal) > 0.0)
    {
        w = -w;
    }
    Vector3 u;
    if (fabs(w.x()) < 0.01)
    {
        u = cross(Vector3(1.0, 0.0, 0.0), w).norm();
    }
    else
    {
        u = cross(Vector3(0.0, 1.0, 0.0), w).norm();
    }
    Vector3 v = cross(w, u);
    Vector3 b = (cos(phi) * sintheta * u +
                 sin(phi) * sintheta * v +
                 costheta * w).norm();
    Ray r(record.point, b);
    Vector3 nl = record.normal;
    if (dot(ray.d(), record.normal) > 0.0)
    {
        nl = -nl;
    }
//    Color total;
//    for (int i = 0; i < _shapeNum; ++i)
//    {
//        Shape* s = _shapes[i];
//        if (s->emission().r() < 1e-6 && s->emission().g() < 1e-6 && s->emission().b() < 1e-6)
//        {
//            continue;
//        }
//        Vector3 w = -record.normal;
//        Vector3 u = cross(Vector3(1.0, 0.0, 0.0), w).norm();
//        if (fabs(w.x()) > 1e-4)
//        {
//            u = cross(Vector3(0.0, 1.0, 0.0), w).norm();
//        }
//        Vector3 v = cross(w, u);
//        Vector3 x = record.point;
//        Vector3 sc = ((Sphere*)s)->c();
//        double sr = ((Sphere*)s)->r();
//        double cosAlphaMax = sqrt(1.0 - sr * sr / dot(x - sc, x - sc));
//        double r1 = random();
//        double r2 = random();
//        double cosAlpha = 1.0 + r1 * (cosAlphaMax - 1);
//        double sinAlpha = sqrt(1.0 - cosAlpha * cosAlpha);
//        double phi = 2 * PI * r2;
//        Vector3 a = (u * cos(phi)*sinAlpha +
//                     v * sin(phi)*sinAlpha +
//                     w * cosAlpha).norm();
//        HitRecord temp;
//        int index = -1;
//        double t = 1e100;
//        for (int j = 0; j < _shapeNum; ++j)
//        {
//            if (_shapes[j]->hitTest(Ray(x, a), temp))
//            {
//                if (temp.t < t)
//                {
//                    t = temp.t;
//                    index = j;
//                }
//            }
//        }
//        if (index == i)
//        {
//            double omega = 2.0 * PI * (1 - cosAlphaMax);
//            total = total + color * (s->emission() * dot(a, nl) * omega) / PI;
//        }
//    }
//    return shape->emission() * emit + total + color * radiance(r, depth + 1, false);
    return shape->emission() + color * radiance(r, depth + 1);
}

Color Radiance::idealSpecular(const Ray& ray, const HitRecord& record, Shape* shape, const Color& color, int depth)
{
    Vector3 d = (ray.d() - 2.0 * dot(ray.d(), record.normal) * record.normal).norm();
    Ray r(record.point, d);
    return shape->emission() + color * radiance(r, depth + 1);
}

Color Radiance::idealRefraction(const Ray& ray, const HitRecord& record, Shape* shape, const Color& color, int depth)
{
    Vector3 d = ray.d();
    Vector3 n = record.normal;
    double ddn = dot(d, n);

    Vector3 dr = (d - 2.0 * ddn * n).norm();
    Ray r(record.point, dr);

    bool into = dot(ray.d(), n) < 0.0;
    if (!into)
    {
        n = -n;
    }
    ddn = dot(d, n);

    double nc = 1.0;
    double nt = shape->dielectric();
    double nnt = into ? nc / nt : nt / nc;
    double costheta2 = 1.0 - nnt * nnt * (1.0 - ddn * ddn);
    if (costheta2 < 0.0)
    {
        return shape->emission() + color * radiance(r, depth + 1);
    }
    double costheta = sqrt(costheta2);
    Vector3 td = (nnt * (d - n * ddn) - n * costheta).norm();
    Ray t(record.point, td);
    double a = nt - nc;
    double b = nt + nc;
    double R0 = a * a / (b * b);
    double c = 1.0 + ddn;
    if (!into)
    {
        c = 1.0 - dot(td, record.normal);
    }
    double Rtheta = R0 + (1.0 - R0) * c * c * c * c * c;
    double k = 0.5;
    double P = k * 0.5 + (1.0 - k) * Rtheta;
    if (random() < P)
    {
        return shape->emission() + color * Rtheta * radiance(r, depth + 1) / P;
    }
    return shape->emission() + color * (1.0 - Rtheta) * radiance(t, depth + 1) / (1.0 - P);
}
