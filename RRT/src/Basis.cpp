#include "Basis.h"

Basis::Basis()
{
}

Basis::Basis(const Vector3& origin, const Vector3& u, const Vector3&v)
{
}

Basis::~Basis()
{
}

const Vector3& Basis::o() const
{
    return _o;
}

const Vector3& Basis::u() const
{
    return _u;
}

const Vector3& Basis::v() const
{
    return _v;
}

const Vector3& Basis::w() const
{
    return _w;
}

Vector3 Basis::at(float x, float y, float z)
{
    return o() + x * u() + y * v() + z * w();
}
