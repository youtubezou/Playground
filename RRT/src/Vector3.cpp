#include "Vector3.h"
using namespace std;

ostream& operator<<(ostream& out, const Vector3& vec) {
    out << '(' << vec.x() << ' ' << vec.y() << ' ' << vec.z() << ')';
    return out;
}
