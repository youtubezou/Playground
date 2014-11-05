#include "Vector3.h"
using namespace std;

Vector3::Vector3(double x, double y, double z)
{
    _d[0] = x;
    _d[1] = y;
    _d[2] = z;
}

Vector3::~Vector3()
{
}

ostream& operator<<(ostream& out, const Vector3& vec)
{
    out << '(' << vec.x() << ' ' << vec.y() << ' ' << vec.z() << ')';
    return out;
}
