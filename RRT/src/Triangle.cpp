#include "Triangle.h"

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c) :
    _a(a), _b(b), _c(c), _n(),
    _uva(0.0, 0.0), _uvb(1.0, 0.0), _uvc(0.0, 1.0) {
    _n = cross(b - a, c - a).norm();
}

Triangle::~Triangle() {
}

bool Triangle::hitTest(const Ray& ray, HitRecord& record) const {
    double a = _a.x() - _b.x();
    double b = _a.y() - _b.y();
    double c = _a.z() - _b.z();

    double d = _a.x() - _c.x();
    double e = _a.y() - _c.y();
    double f = _a.z() - _c.z();

    double g = ray.d().x();
    double h = ray.d().y();
    double i = ray.d().z();

    double j = _a.x() - ray.o().x();
    double k = _a.y() - ray.o().y();
    double l = _a.z() - ray.o().z();

    double ei_hf = e * i - h * f;
    double gf_di = g * f - d * i;
    double dh_eg = d * h - e * g;

    double ak_jb = a * k - j * b;
    double jc_al = j * c - a * l;
    double bl_kc = b * l - k * c;

    double M = a * ei_hf + b * gf_di + c * dh_eg;

    double beta = (j * ei_hf + k * gf_di + l * dh_eg) / M;
    if (beta < 0.0 || beta > 1.0) {
        return false;
    }
    double gamma = (i * ak_jb + h * jc_al + g * bl_kc) / M;
    if (gamma < 0.0 || beta + gamma > 1.0) {
        return false;
    }
    record.t = - (f * ak_jb + e * jc_al + d * bl_kc) / M;
    if (record.t  < 1e-5) {
        return false;
    }
    record.point = ray.at(record.t);
    record.normal = _n;
    record.uv = uva() + beta * (uvb() - uva()) + gamma * (uvc() - uva());
    record.texture = texture();
    return true;
}
