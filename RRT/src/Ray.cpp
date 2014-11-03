#include "Ray.h"
using namespace std;

Ray::Ray() :
    _o(), _d()
{
}

Ray::Ray(const Vector3& origin, const Vector3& direction) :
    _o(origin), _d(direction)
{
}

Ray::~Ray()
{
}

Vector3 Ray::at(float t) const
{
    return o() + t * d();
}

ostream& operator<<(ostream& out, const Ray& ray)
{
    out << ray.o() << " + t * " << ray.d();
    return out;
}
