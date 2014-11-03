#include "Camera.h"

Camera::Camera(const Vector3& center, const Vector3& gaze, const Vector3& vup,
               float aperture, float distance, float u0, float v0, float u1, float v1) :
    _center(center), _gaze(gaze),
    _u(), _v(), _w(),
    _o(), _a(), _b(),
    _radius(0.0f)
{
    _radius = aperture / 2.0f;
    _w = - _gaze.norm();
    _u = cross(vup, _w).norm();
    _v = cross(_w, _u);
    _o = center - distance * _w + u0 * _u + v0 * _v;
    _a = (u1 - u0) * _u;
    _b = (v1 - v0) * _v;
}

Camera::~Camera()
{
}

Ray Camera::getRay(float a, float b, float ra, float rb)
{
    Vector3 origin = _center + 2.0f * (ra - 0.5f) * _radius * _u +
                               2.0f * (rb - 0.5f) * _radius * _v;
    Vector3 target = _o + a * _a + b * _b;
    return Ray(origin, (target - origin).norm());
}
