#include "Ray.h"
using namespace std;

ostream& operator<<(ostream& out, const Ray& ray) {
    out << ray.o() << " + t * " << ray.d();
    return out;
}
