#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "Vector3.h"

class Ray
{
public:
    Ray();
    Ray(const Vector3& origin, const Vector3& direction);
    virtual ~Ray();

    const Vector3& o() const;
    const Vector3& d() const;

    Vector3 at(double t) const;

    friend std::ostream& operator<<(std::ostream& out, const Ray& ray);

private:
    Vector3 _o; // Origin
    Vector3 _d; // Direction
};

inline const Vector3& Ray::o() const
{
    return _o;
}

inline const Vector3& Ray::d() const
{
    return _d;
}

#endif // RAY_H
