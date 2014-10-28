#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray
{
public:
    Ray();
    Ray(const Vector3& a, const Vector3& b);
    Ray(const Ray& ray);

    Vector3 origin() const;
    Vector3 direction() const;
    Vector3 pointAt(float t) const;

    friend std::ostream& operator <<(std::ostream &os, const Ray& ray);

private:
    Vector3 _data[2];
};

inline Ray::Ray()
{
}

inline Ray::Ray(const Vector3& a, const Vector3& b)
{
    _data[0] = a;
    _data[1] = b;
}

inline Ray::Ray(const Ray& ray)
{
    *this = ray;
}

inline Vector3 Ray::origin() const
{
    return _data[0];
}

inline Vector3 Ray::direction() const
{
    return _data[1];
}

inline Vector3 Ray::pointAt(float t) const
{
    return _data[0] + t * _data[1];
}

inline std::ostream& operator <<(std::ostream &os, const Ray& ray)
{
    os << '(' << ray._data[0] << ") + t (" << ray._data[1] << ')';
    return os;
}

#endif // RAY_H
