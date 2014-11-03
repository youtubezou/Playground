#ifndef BASIS_H
#define BASIS_H

#include "Vector3.h"

class Basis
{
public:
    Basis();
    Basis(const Vector3& origin, const Vector3& u, const Vector3&v);
    virtual ~Basis();

    const Vector3& o() const;
    const Vector3& u() const;
    const Vector3& v() const;
    const Vector3& w() const;

    Vector3 at(float x, float y, float z);

private:
    Vector3 _o;
    Vector3 _u;
    Vector3 _v;
    Vector3 _w;
};

#endif // BASIS_H
