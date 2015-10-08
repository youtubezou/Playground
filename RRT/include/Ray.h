#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "Vector3.h"

class Ray {
public:
    constexpr Ray();
    constexpr Ray(const Vector3& origin, const Vector3& direction);

    constexpr const Vector3& o();
    constexpr const Vector3& d();

    constexpr Vector3 at(double t) const;

    friend std::ostream& operator<<(std::ostream& out, const Ray& ray);

private:
    Vector3 _o; // Origin
    Vector3 _d; // Direction
};

constexpr Ray::Ray() : _o(), _d() {
}

constexpr Ray::Ray(const Vector3& origin, const Vector3& direction) :
    _o(origin), _d(direction) {
}

inline constexpr const Vector3& Ray::o() {
    return _o;
}

inline constexpr const Vector3& Ray::d() {
    return _d;
}

constexpr Vector3 Ray::at(double t) {
    return o() + t * d();
}

#endif // RAY_H
