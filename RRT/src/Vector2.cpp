#include "Vector2.h"
using namespace std;

istream& operator >>(istream& is, Vector2 &v)
{
    is >> v._e[0] >> v._e[1];
    return is;
}

ostream& operator <<(ostream& os, const Vector2 &v)
{
    os << '(' << v._e[0] << ' ' << v._e[1] << ')';
    return os;
}
