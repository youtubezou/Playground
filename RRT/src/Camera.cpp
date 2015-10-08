#include "Camera.h"

Camera::Camera(const Vector3& center, const Vector3& gaze, const Vector3& vup,
               double aperture, double distance, double u0, double v0, double u1, double v1) :
    _center(center), _gaze(gaze),
    _u(), _v(), _w(),
    _o(), _a(), _b(),
    _radius(0.0) {
    _radius = aperture / 2.0;
    _w = - _gaze.norm();
    _u = cross(vup, _w).norm();
    _v = cross(_w, _u);
    _o = center - distance * _w + u0 * _u + v0 * _v;
    _a = (u1 - u0) * _u;
    _b = (v1 - v0) * _v;
}

Ray Camera::getRay(double a, double b, double ra, double rb) {
    Vector3 origin = _center + 2.0 * (ra - 0.5) * _radius * _u +
                     2.0 * (rb - 0.5) * _radius * _v;
    Vector3 target = _o + a * _a + b * _b;
    return Ray(origin, (target - origin).norm());
}
