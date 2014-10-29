#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>

class Vector2
{
public:
    Vector2();
    Vector2(float e0, float e1);
    Vector2(const Vector2 &v);

    float x() const;
    float y() const;

    void setX(float x);
    void setY(float y);

    float operator [](int i) const;
    float& operator [](int i);

    friend bool operator ==(const Vector2& v1, const Vector2& v2);
    friend bool operator !=(const Vector2& v1, const Vector2& v2);

    const Vector2& operator +() const;
    Vector2 operator -() const;
    friend Vector2 operator *(float scalar, const Vector2& v);
    friend Vector2 operator *(const Vector2& v, float scalar);
    friend Vector2 operator /(const Vector2& v, float scalar);
    friend Vector2 operator +(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator -(const Vector2& v1, const Vector2& v2);
    Vector2& operator =(const Vector2& v);
    Vector2& operator *=(const float scalar);
    Vector2& operator /=(const float scalar);
    Vector2& operator +=(const Vector2& v);
    Vector2& operator -=(const Vector2& v);

    friend std::istream& operator >>(std::istream& is, Vector2 &v);
    friend std::ostream& operator <<(std::ostream& os, const Vector2 &v);

    float length() const;
    float squaredLength() const;

    void makeUnitVector();

    float minComponent() const;
    float maxComponent() const;
    float minAbsComponent() const;
    float maxAbsComponent() const;
    int indexOfMinComponent() const;
    int indexOfMaxComponent() const;
    int indexOfMinAbsComponent() const;
    int indexOfMaxAbsComponent() const;

    friend Vector2 unitVector(const Vector2& v);
    friend Vector2 minVec(const Vector2& v1, const Vector2& v2);
    friend Vector2 maxVec(const Vector2& v1, const Vector2& v2);
    friend float dot(const Vector2& v1, const Vector2& v2);
    friend float cross(const Vector2& v1, const Vector2& v2);

private:
    float _e[2];
};

inline Vector2::Vector2()
{
}

inline Vector2::Vector2(float e0, float e1)
{
    _e[0] = e0;
    _e[1] = e1;
}

inline Vector2::Vector2(const Vector2 &v)
{
    *this = v;
}

inline float Vector2::x() const
{
    return _e[0];
}

inline float Vector2::y() const
{
    return _e[1];
}

inline void Vector2::setX(float x)
{
    _e[0] = x;
}

inline void Vector2::setY(float y)
{
    _e[1] = y;
}

inline float Vector2::operator [](int i) const
{
    return _e[i];
}

inline float& Vector2::operator [](int i)
{
    return _e[i];
}

inline bool operator ==(const Vector2& v1, const Vector2& v2)
{
    if (fabs(v1._e[0] - v2._e[0]) > 1e-6)
    {
        return false;
    }
    if (fabs(v1._e[1] - v2._e[1]) > 1e-6)
    {
        return false;
    }
    return true;
}

inline bool operator !=(const Vector2& v1, const Vector2& v2)
{
    return !(v1 == v2);
}

inline const Vector2& Vector2::operator +() const
{
    return *this;
}

inline Vector2 Vector2::operator -() const
{
    return Vector2(-_e[0], -_e[1]);
}

inline Vector2 operator *(float scalar, const Vector2& v)
{
    return Vector2(v._e[0]*scalar, v._e[1]*scalar);
}

inline Vector2 operator *(const Vector2& v, float scalar)
{
    return Vector2(v._e[0]*scalar, v._e[1]*scalar);
}

inline Vector2 operator /(const Vector2& v, float scalar)
{
    return Vector2(v._e[0]/scalar, v._e[1]/scalar);
}

inline Vector2 operator +(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1._e[0]+v2._e[0], v1._e[1]+v2._e[1]);
}

inline Vector2 operator -(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1._e[0]-v2._e[0], v1._e[1]-v2._e[1]);
}

inline Vector2& Vector2::operator =(const Vector2& v)
{
    _e[0] = v._e[0];
    _e[1] = v._e[1];
    return *this;
}

inline Vector2& Vector2::operator *=(const float scalar)
{
    *this = *this * scalar;
    return *this;
}

inline Vector2& Vector2::operator /=(const float scalar)
{
    *this = *this / scalar;
    return *this;
}

inline Vector2& Vector2::operator +=(const Vector2& v)
{
    *this = *this + v;
    return *this;
}

inline Vector2& Vector2::operator -=(const Vector2& v)
{
    *this = *this - v;
    return *this;
}

inline float Vector2::length() const
{
    return sqrt(_e[0]*_e[0] + _e[1]*_e[1]);
}

inline float Vector2::squaredLength() const
{
    return _e[0]*_e[0] + _e[1]*_e[1];
}

inline void Vector2::makeUnitVector()
{
    *this = *this / length();
}

inline float Vector2::minComponent() const
{
    return _e[0] < _e[1] ? _e[0] : _e[1];
}

inline float Vector2::maxComponent() const
{
    return _e[0] > _e[1] ? _e[0] : _e[1];
}

inline float Vector2::minAbsComponent() const
{
    return fabs(_e[0]) < fabs(_e[1]) ? fabs(_e[0]) : fabs(_e[1]);
}

inline float Vector2::maxAbsComponent() const
{
    return fabs(_e[0]) > fabs(_e[1]) ? fabs(_e[0]) : fabs(_e[1]);
}

inline int Vector2::indexOfMinComponent() const
{
    return _e[0] < _e[1] ? 0 : 1;
}

inline int Vector2::indexOfMaxComponent() const
{
    return _e[0] > _e[1] ? 0 : 1;
}

inline int Vector2::indexOfMinAbsComponent() const
{
    return fabs(_e[0]) < fabs(_e[1]) ? 0 : 1;
}

inline int Vector2::indexOfMaxAbsComponent() const
{
    return fabs(_e[0]) > fabs(_e[1]) ? 0 : 1;
}

inline Vector2 unitVector(const Vector2& v)
{
    return v / v.length();
}

inline Vector2 minVec(const Vector2& v1, const Vector2& v2)
{
    Vector2 temp(v1);
    if (v2._e[0] < v1._e[0])
    {
        temp._e[0] = v2._e[0];
    }
    if (v2._e[1] < v1._e[1])
    {
        temp._e[1] = v2._e[1];
    }
    return temp;
}

inline Vector2 maxVec(const Vector2& v1, const Vector2& v2)
{
    Vector2 temp(v1);
    if (v2._e[0] > v1._e[0])
    {
        temp._e[0] = v2._e[0];
    }
    if (v2._e[1] > v1._e[1])
    {
        temp._e[1] = v2._e[1];
    }
    return temp;
}

inline float dot(const Vector2& v1, const Vector2& v2)
{
    return v1._e[0]*v2._e[0] + v1._e[1]*v2._e[1];
}

inline float cross(const Vector2& v1, const Vector2& v2)
{
    return v1._e[0]*v2._e[1] - v1._e[1]*v2._e[0];
}

#endif // VECTOR2_H
