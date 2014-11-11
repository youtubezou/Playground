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

    const Vector2& uva() const;
    const Vector2& uvb() const;
    const Vector2& uvc() const;
    void setUva(const Vector2& uv);
    void setUvb(const Vector2& uv);
    void setUvc(const Vector2& uv);

    virtual bool hitTest(const Ray& ray, HitRecord& record) const override;

private:
    Vector3 _a, _b, _c, _n;
    Vector2 _uva, _uvb, _uvc;
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

inline const Vector2& Triangle::uva() const
{
    return _uva;
}

inline const Vector2& Triangle::uvb() const
{
    return _uvb;
}

inline const Vector2& Triangle::uvc() const
{
    return _uvc;
}

inline void Triangle::setUva(const Vector2& uv)
{
    _uva = uv;
}

inline void Triangle::setUvb(const Vector2& uv)
{
    _uvb = uv;
}

inline void Triangle::setUvc(const Vector2& uv)
{
    _uvc = uv;
}

#endif // TRIANGLE_H
