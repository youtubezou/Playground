#include "Vector2.h"
using namespace std;

Vector2::Vector2(double x, double y)
{
    _d[0] = x;
    _d[1] = y;
}

Vector2::~Vector2()
{
}

ostream& operator<<(ostream& out, const Vector2& vec)
{
    out << '(' << vec.x() << ' ' << vec.y() << ')';
    return out;
}
