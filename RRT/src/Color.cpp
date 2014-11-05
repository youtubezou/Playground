#include "Color.h"
using namespace std;

Color::Color(double r, double g, double b)
{
    _d[0] = r;
    _d[1] = g;
    _d[2] = b;
}

Color::~Color()
{
}

ostream& operator<<(ostream& out, const Color& vec)
{
    out << '[' << vec.r() << ' ' << vec.g() << ' ' << vec.b() << ']';
    return out;
}
