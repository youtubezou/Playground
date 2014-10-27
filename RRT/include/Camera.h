#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Ray.h"

class Camera {
public:
    Camera(const Vector3& center, const Vector3& gaze, const Vector3& vup,
           double aperture, double distance, double u0, double v0, double u1, double v1);
    virtual ~Camera() = default;

    Ray getRay(double a, double b, double ra, double rb);

private:
    Vector3 _center, _gaze;
    Vector3 _u, _v, _w;
    Vector3 _o, _a, _b;
    double _radius;
};

#endif // CAMERA_H
