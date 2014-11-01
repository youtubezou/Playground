#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "OrthonormalBasis.h"

class Camera
{
public:
    Camera();
    Camera(const Camera& camera);
    Camera(Vector3 center, Vector3 gaze, Vector3 up, float aperture, float u0, float v0, float u1, float v1, float distance);
    Ray getRay(float a, float b, float xi1, float xi2);

private:
    Vector3 _center;
    Vector3 _corner;
    Vector3 _across;
    Vector3 _up;
    OrthonormalBasis _uvw;
    float _radius;
    float _u0, _v0, _u1, _v1;
    float _distance;
};

#endif // CAMERA_H
