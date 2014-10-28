#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3
{
public:
    Vector3();
    Vector3(float e0, float e1, float e2);
    Vector3(const Vector3 &v);

    float x() const;
    float y() const;
    float z() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    float operator [](int i) const;
    float& operator [](int i);

    friend bool operator ==(const Vector3& v1, const Vector3& v2);
    friend bool operator !=(const Vector3& v1, const Vector3& v2);

    const Vector3& operator +() const;
    Vector3 operator -() const;
    friend Vector3 operator *(float scalar, const Vector3& v);
    friend Vector3 operator *(const Vector3& v, float scalar);
    friend Vector3 operator /(const Vector3& v, float scalar);
    friend Vector3 operator +(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator -(const Vector3& v1, const Vector3& v2);
    Vector3& operator =(const Vector3& v);
    Vector3& operator *=(const float scalar);
    Vector3& operator /=(const float scalar);
    Vector3& operator +=(const Vector3& v);
    Vector3& operator -=(const Vector3& v);

    friend std::istream& operator >>(std::istream& is, Vector3 &v);
    friend std::ostream& operator <<(std::ostream& os, const Vector3 &v);

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

    friend Vector3 unitVector(const Vector3& v);
    friend Vector3 minVec(const Vector3& v1, const Vector3& v2);
    friend Vector3 maxVec(const Vector3& v1, const Vector3& v2);
    friend float dot(const Vector3& v1, const Vector3& v2);
    friend Vector3 cross(const Vector3& v1, const Vector3& v2);
    friend float tripleProduct(const Vector3& v1, const Vector3& v2, const Vector3& v3);

private:
    float _e[3];
};

inline Vector3::Vector3()
{
}

inline Vector3::Vector3(float e0, float e1, float e2)
{
    _e[0] = e0;
    _e[1] = e1;
    _e[2] = e2;
}

inline Vector3::Vector3(const Vector3 &v)
{
    *this = v;
}

inline float Vector3::x() const
{
    return _e[0];
}

inline float Vector3::y() const
{
    return _e[1];
}

inline float Vector3::z() const
{
    return _e[2];
}

inline void Vector3::setX(float x)
{
    _e[0] = x;
}

inline void Vector3::setY(float y)
{
    _e[1] = y;
}

inline void Vector3::setZ(float z)
{
    _e[2] = z;
}

inline float Vector3::operator [](int i) const
{
    return _e[i];
}

inline float& Vector3::operator [](int i)
{
    return _e[i];
}

inline bool operator ==(const Vector3& v1, const Vector3& v2)
{
    if (fabs(v1._e[0] - v2._e[0]) > 1e-6)
    {
        return false;
    }
    if (fabs(v1._e[1] - v2._e[1]) > 1e-6)
    {
        return false;
    }
    if (fabs(v1._e[2] - v2._e[2]) > 1e-6)
    {
        return false;
    }
    return true;
}

inline bool operator !=(const Vector3& v1, const Vector3& v2)
{
    return !(v1 == v2);
}

inline const Vector3& Vector3::operator +() const
{
    return *this;
}

inline Vector3 Vector3::operator -() const
{
    return Vector3(-_e[0], -_e[1], -_e[2]);
}

inline Vector3 operator *(float scalar, const Vector3& v)
{
    return Vector3(v._e[0]*scalar, v._e[1]*scalar, v._e[2]*scalar);
}

inline Vector3 operator *(const Vector3& v, float scalar)
{
    return Vector3(v._e[0]*scalar, v._e[1]*scalar, v._e[2]*scalar);
}

inline Vector3 operator /(const Vector3& v, float scalar)
{
    return Vector3(v._e[0]/scalar, v._e[1]/scalar, v._e[2]/scalar);
}

inline Vector3 operator +(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1._e[0]+v2._e[0], v1._e[1]+v2._e[1], v1._e[2]+v2._e[2]);
}

inline Vector3 operator -(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1._e[0]-v2._e[0], v1._e[1]-v2._e[1], v1._e[2]-v2._e[2]);
}

inline Vector3& Vector3::operator =(const Vector3& v)
{
    _e[0] = v._e[0];
    _e[1] = v._e[1];
    _e[2] = v._e[2];
    return *this;
}

inline Vector3& Vector3::operator *=(const float scalar)
{
    *this = *this * scalar;
    return *this;
}

inline Vector3& Vector3::operator /=(const float scalar)
{
    *this = *this / scalar;
    return *this;
}

inline Vector3& Vector3::operator +=(const Vector3& v)
{
    *this = *this + v;
    return *this;
}

inline Vector3& Vector3::operator -=(const Vector3& v)
{
    *this = *this - v;
    return *this;
}

inline float Vector3::length() const
{
    return sqrt(_e[0]*_e[0] + _e[1]*_e[1] + _e[2]*_e[2]);
}

inline float Vector3::squaredLength() const
{
    return _e[0]*_e[0] + _e[1]*_e[1] + _e[2]*_e[2];
}

inline void Vector3::makeUnitVector()
{
    *this = *this / length();
}

inline float Vector3::minComponent() const
{
    float temp = _e[0];
    if (_e[1] < temp)
    {
        temp = _e[1];
    }
    if (_e[2] < temp)
    {
        temp = _e[2];
    }
    return temp;
}

inline float Vector3::maxComponent() const
{
    float temp = _e[0];
    if (_e[1] > temp)
    {
        temp = _e[1];
    }
    if (_e[2] > temp)
    {
        temp = _e[2];
    }
    return temp;
}

inline float Vector3::minAbsComponent() const
{
    float temp = fabs(_e[0]);
    if (fabs(_e[1]) < temp)
    {
        temp = fabs(_e[1]);
    }
    if (fabs(_e[2]) < temp)
    {
        temp = fabs(_e[2]);
    }
    return temp;
}

inline float Vector3::maxAbsComponent() const
{
    float temp = fabs(_e[0]);
    if (fabs(_e[1]) > temp)
    {
        temp = fabs(_e[1]);
    }
    if (fabs(_e[2]) > temp)
    {
        temp = fabs(_e[2]);
    }
    return temp;
}

inline int Vector3::indexOfMinComponent() const
{
    int index = 0;
    if (_e[1] < _e[index])
    {
        index = 1;
    }
    if (_e[2] < _e[index])
    {
        index = 2;
    }
    return index;
}

inline int Vector3::indexOfMaxComponent() const
{
    int index = 0;
    if (_e[1] > _e[index])
    {
        index = 1;
    }
    if (_e[2] > _e[index])
    {
        index = 2;
    }
    return index;
}

inline int Vector3::indexOfMinAbsComponent() const
{
    int index = 0;
    if (fabs(_e[1]) < fabs(_e[index]))
    {
        index = 1;
    }
    if (fabs(_e[2]) < fabs(_e[index]))
    {
        index = 2;
    }
    return index;
}

inline int Vector3::indexOfMaxAbsComponent() const
{
    int index = 0;
    if (fabs(_e[1]) > fabs(_e[index]))
    {
        index = 1;
    }
    if (fabs(_e[2]) > fabs(_e[index]))
    {
        index = 2;
    }
    return index;
}

inline Vector3 unitVector(const Vector3& v)
{
    return v / v.length();
}

inline Vector3 minVec(const Vector3& v1, const Vector3& v2)
{
    Vector3 temp(v1);
    if (v2._e[0] < v1._e[0])
    {
        temp._e[0] = v2._e[0];
    }
    if (v2._e[1] < v1._e[1])
    {
        temp._e[1] = v2._e[1];
    }
    if (v2._e[2] < v1._e[2])
    {
        temp._e[2] = v2._e[2];
    }
    return temp;
}

inline Vector3 maxVec(const Vector3& v1, const Vector3& v2)
{
    Vector3 temp(v1);
    if (v2._e[0] > v1._e[0])
    {
        temp._e[0] = v2._e[0];
    }
    if (v2._e[1] > v1._e[1])
    {
        temp._e[1] = v2._e[1];
    }
    if (v2._e[2] > v1._e[2])
    {
        temp._e[2] = v2._e[2];
    }
    return temp;
}

inline float dot(const Vector3& v1, const Vector3& v2)
{
    return v1._e[0]*v2._e[0] + v1._e[1]*v2._e[1] + v1._e[2]*v2._e[2];
}

inline Vector3 cross(const Vector3& v1, const Vector3& v2)
{
    Vector3 temp;
    temp._e[0] = v1._e[1] * v2._e[2] - v1._e[2] * v2._e[1];
    temp._e[1] = v1._e[2] * v2._e[0] - v1._e[0] * v2._e[2];
    temp._e[2] = v1._e[0] * v2._e[1] - v1._e[1] * v2._e[0];
    return temp;
}

#endif // VECTOR3_H
