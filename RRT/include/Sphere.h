#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
    Sphere(const Location& center, double radius);

    const Location& c() const;
    double r() const;

    virtual bool hitTest(const Ray& ray, HitRecord& record) const override;

private:
    Location _c;
    double _r;
};

inline const Location& Sphere::c() const {
    return _c;
}

inline double Sphere::r() const {
    return _r;
}

#endif // SPHERE_H
