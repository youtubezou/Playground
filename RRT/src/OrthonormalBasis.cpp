#include "OrthonormalBasis.h"
using namespace std;

#define ONB_EPSILON 0.01f

OrthonormalBasis::OrthonormalBasis() :
    _u(), _v(), _w()
{
}

OrthonormalBasis::OrthonormalBasis(const Vector3& a, const Vector3& b, const Vector3& c) :
    _u(a), _v(b), _w(c)
{
}

Vector3 OrthonormalBasis::u() const
{
    return _u;
}

Vector3 OrthonormalBasis::v() const
{
    return _v;
}

Vector3 OrthonormalBasis::w() const
{
    return _w;
}

void OrthonormalBasis::set(const Vector3& a, const Vector3& b, const Vector3& c)
{
    _u = a;
    _v = b;
    _w = c;
}

void OrthonormalBasis::initFromU(const Vector3& u)
{
    Vector3 m(0.0f, 1.0f, 0.0f);
    Vector3 n(1.0f, 0.0f, 0.0f);
    _u = unitVector(u);
    _v = cross(_u, n);
    if (_v.length() < ONB_EPSILON)
    {
        _v = cross(_u, m);
    }
    _w = cross(_u, _v);
}

void OrthonormalBasis::initFromV(const Vector3& v)
{
    Vector3 m(0.0f, 1.0f, 0.0f);
    Vector3 n(1.0f, 0.0f, 0.0f);
    _v = unitVector(v);
    _u = cross(_v, n);
    if (_u.length() < ONB_EPSILON)
    {
        _u = cross(_v, m);
    }
    _w = cross(_u, _v);
}

void OrthonormalBasis::initFromW(const Vector3& w)
{
    Vector3 m(0.0f, 1.0f, 0.0f);
    Vector3 n(1.0f, 0.0f, 0.0f);
    _w = unitVector(w);
    _u = cross(_w, n);
    if (_u.length() < ONB_EPSILON)
    {
        _u = cross(_w, m);
    }
    _v = cross(_w, _u);
}

void OrthonormalBasis::initFromUV(const Vector3& u, const Vector3& v)
{
    _u = unitVector(u);
    _w = unitVector(cross(u, v));
    _v = cross(_w, _u);
}

void OrthonormalBasis::initFromVU(const Vector3& v, const Vector3& u)
{
    _v = unitVector(u);
    _w = unitVector(cross(u, v));
    _u = cross(_v, _w);
}

void OrthonormalBasis::initFromUW(const Vector3& u, const Vector3& w)
{
    _u = unitVector(u);
    _v = unitVector(cross(w, u));
    _w = cross(_u, _v);
}

void OrthonormalBasis::initFromWU(const Vector3& w, const Vector3& u)
{
    _w = unitVector(w);
    _v = unitVector(cross(w, u));
    _u = cross(_v, _w);
}

void OrthonormalBasis::initFromVW(const Vector3& v, const Vector3& w)
{
    _v = unitVector(v);
    _u = unitVector(cross(v, w));
    _w = cross(_u, _v);
}

void OrthonormalBasis::initFromWV(const Vector3& w, const Vector3& v)
{
    _w = unitVector(w);
    _u = unitVector(cross(v, w));
    _v = cross(_w, _u);
}

bool operator ==(const OrthonormalBasis& basis1, const OrthonormalBasis& basis2)
{
    return basis1.u() == basis2.u() && basis1.v() == basis2.v() && basis1.w() == basis2.w();
}

istream &operator >>(istream &is, OrthonormalBasis& basis)
{
    is >> basis._u >> basis._v >> basis._w;
    return is;
}

ostream &operator <<(ostream &os, const OrthonormalBasis& basis)
{
    os << basis._u << endl << basis._v << endl << basis._w << endl;
    return os;
}
