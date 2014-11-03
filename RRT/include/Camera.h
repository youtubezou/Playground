#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Ray.h"

class Camera
{
public:
    Camera(const Vector3& center, const Vector3& gaze, const Vector3& vup,
           float aperture, float distance, float u0, float v0, float u1, float v1);
    virtual ~Camera();

    Ray getRay(float a, float b, float ra, float rb);

private:
    Vector3 _center, _gaze;
    Vector3 _u, _v, _w;
    Vector3 _o, _a, _b;
    float _radius;
};

#endif // CAMERA_H
