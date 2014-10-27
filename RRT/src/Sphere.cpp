#include <cmath>
#include "Sphere.h"
const double PI = acos(-1.0);

Sphere::Sphere(const Vector3& center, double radius) :
    _c(center), _r(radius) {
}

bool Sphere::hitTest(const Ray& ray, HitRecord& record) const {
    Vector3 o_c = ray.o() - _c;
    double a = dot(ray.d(), ray.d());
    double b = dot(2.0 * ray.d(), o_c);
    double c = dot(o_c, o_c) - _r * _r;;
    double discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0) {
        return false;
    }
    discriminant = sqrt(discriminant);
    double t = (- b - discriminant) / (2.0 * a);
    if (t < 1e-4) {
        t = (- b + discriminant) / (2.0 * a);
    }
    if (t < 1e-4) {
        return false;
    }
    record.t = t;
    record.point = ray.at(t);
    record.normal = (record.point - _c).norm();
    double phi = atan2(record.normal.x(), record.normal.z());
    if (phi < 0.0) {
        phi += PI * 2.0;
    }
    double theta = acos(record.normal.y());
    record.uv = Vector2((PI * 2.0 - phi) / (PI * 2.0), theta / PI);
    record.texture = texture();
    return true;
}
