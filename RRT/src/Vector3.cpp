#include "Vector3.h"
using namespace std;

istream& operator >>(istream& is, Vector3 &v)
{
    is >> v._e[0] >> v._e[1] >> v._e[2];
    return is;
}

ostream& operator <<(ostream& os, const Vector3 &v)
{
    os << '(' << v._e[0] << ' ' << v._e[1] << ' ' << v._e[2] << ')';
    return os;
}

float tripleProduct(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
    return dot(cross(v1, v2), v3);
}
