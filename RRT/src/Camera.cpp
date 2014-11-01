#include "Camera.h"

Camera::Camera() :
    _center(), _corner(), _across(), _up(), _uvw(),
    _radius(0.0f), _u0(0.0f), _v0(0.0f), _u1(0.0f), _v1(0.0f),
    _distance(0.0f)
{
}

Camera::Camera(const Camera& camera) :
    _center(), _corner(), _across(), _up(), _uvw(),
    _radius(0.0f), _u0(0.0f), _v0(0.0f), _u1(0.0f), _v1(0.0f),
    _distance(0.0f)
{
    *this = camera;
}

Camera::Camera(Vector3 center, Vector3 gaze, Vector3 vup, float aperture, float u0, float v0, float u1, float v1, float distance) :
    _center(center), _corner(), _across(), _up(), _uvw(),
    _radius(0.0f), _u0(u0), _v0(v0), _u1(u1), _v1(v1),
    _distance(distance)
{
    _radius = aperture / 2.0f;
    _uvw.initFromWV(-gaze, vup);
    _corner = _center + _u0*_uvw.u() + _v0*_uvw.v() - _distance*_uvw.w();
    _across = (_u1 - _u0) * _uvw.u();
    _up = (_v1 - _v0) * _uvw.v();
}

Ray Camera::getRay(float a, float b, float xi1, float xi2)
{
    Vector3 origin = _center + 2.0f*(xi1 - 0.5f)*_radius*_uvw.u() +
                               2.0f*(xi2 - 0.5f)*_radius*_uvw.v();
    Vector3 target = _corner + _across*a + _up*b;
    return Ray(origin, unitVector(target - origin));
}
