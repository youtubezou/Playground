#include "Color.h"
using namespace std;

ostream& operator<<(ostream& out, const Color& vec) {
    out << '[' << vec.r() << ' ' << vec.g() << ' ' << vec.b() << ']';
    return out;
}
