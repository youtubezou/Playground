#include "Vector2.h"
using namespace std;

ostream& operator<<(ostream& out, const Vector2& vec) {
    out << '(' << vec.x() << ' ' << vec.y() << ')';
    return out;
}
