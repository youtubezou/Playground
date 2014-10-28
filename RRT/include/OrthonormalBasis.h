#ifndef ORTHONORMALBASIS_H
#define ORTHONORMALBASIS_H

#include "Vector3.h"

class OrthonormalBasis
{
public:
    OrthonormalBasis();
    OrthonormalBasis(const Vector3& a, const Vector3& b, const Vector3& c);

    Vector3 u() const;
    Vector3 v() const;
    Vector3 w() const;

    void set(const Vector3& a, const Vector3& b, const Vector3& c);

    void initFromU(const Vector3& u);
    void initFromV(const Vector3& v);
    void initFromW(const Vector3& w);

    void initFromUV(const Vector3& u, const Vector3& v);
    void initFromVU(const Vector3& v, const Vector3& u);

    void initFromUW(const Vector3& u, const Vector3& w);
    void initFromWU(const Vector3& w, const Vector3& u);

    void initFromVW(const Vector3& v, const Vector3& w);
    void initFromWV(const Vector3& w, const Vector3& v);

    friend bool operator ==(const OrthonormalBasis& basis1, const OrthonormalBasis& basis2);

    friend std::istream &operator >>(std::istream &is, OrthonormalBasis& basis);
    friend std::ostream &operator <<(std::ostream &os, const OrthonormalBasis& basis);

private:
    Vector3 _u;
    Vector3 _v;
    Vector3 _w;
};

#endif // ORTHONORMALBASIS_H
