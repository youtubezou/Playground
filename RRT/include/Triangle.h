#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape
{
public:
    Triangle(const Vector3& a = Vector3(), const Vector3& b = Vector3(), const Vector3& c = Vector3());
    virtual ~Triangle();

    const Vector3& a() const;
    const Vector3& b() const;
    const Vector3& c() const;

    virtual bool hitTest(const Ray& ray, HitRecord& record) const override;

private:
    Vector3 _a, _b, _c;
    Vector3 _n;
};

inline const Vector3& Triangle::a() const
{
    return _a;
}

inline const Vector3& Triangle::b() const
{
    return _b;
}

inline const Vector3& Triangle::c() const
{
    return _c;
}

#endif // TRIANGLE_H
