#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3
{
public:
    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    virtual ~Vector3();

    float x() const;
    float y() const;
    float z() const;
    float operator[](int index) const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void set(float x, float y, float z);

    friend Vector3 operator+(const Vector3& a, const Vector3& b);
    friend Vector3 operator-(const Vector3& a, const Vector3& b);
    friend Vector3 operator-(const Vector3& a);
    friend Vector3 operator*(const Vector3& a, const Vector3& b);
    friend Vector3 operator*(const Vector3& a, float b);
    friend Vector3 operator*(float a, const Vector3& b);
    friend Vector3 operator/(const Vector3& a, const Vector3& b);
    friend Vector3 operator/(const Vector3& a, float b);

    friend float dot(const Vector3& a, const Vector3& b);
    friend Vector3 cross(const Vector3& a, const Vector3& b);

    float length() const;
    Vector3 norm() const;

    friend std::ostream& operator<<(std::ostream& out, const Vector3& vec);

private:
    float _d[3];
};

inline float Vector3::x() const
{
    return _d[0];
}

inline float Vector3::y() const
{
    return _d[1];
}

inline float Vector3::z() const
{
    return _d[2];
}

inline float Vector3::operator[](int index) const
{
    return _d[index];
}

inline void Vector3::setX(float x)
{
    _d[0] = x;
}

inline void Vector3::setY(float y)
{
    _d[1] = y;
}

inline void Vector3::setZ(float z)
{
    _d[2] = z;
}

inline void Vector3::set(float x, float y, float z)
{
    setX(x);
    setY(y);
    setZ(z);
}

inline Vector3 operator+(const Vector3& a, const Vector3& b)
{
    return Vector3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

inline Vector3 operator-(const Vector3& a, const Vector3& b)
{
    return Vector3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

inline Vector3 operator-(const Vector3& a)
{
    return Vector3(-a.x(), -a.y(), -a.z());
}

inline Vector3 operator*(const Vector3& a, const Vector3& b)
{
    return Vector3(a.x() * b.x(), a.y() * b.y(), a.z() * b.z());
}

inline Vector3 operator*(const Vector3& a, float b)
{
    return Vector3(a.x() * b, a.y() * b, a.z() * b);
}

inline Vector3 operator*(float a, const Vector3& b)
{
    return Vector3(a * b.x(), a * b.y(), a * b.z());
}

inline Vector3 operator/(const Vector3& a, const Vector3& b)
{
    return Vector3(a.x() / b.x(), a.y() / b.y(), a.z() / b.z());
}

inline Vector3 operator/(const Vector3& a, float b)
{
    return Vector3(a.x() / b, a.y() / b, a.z() / b);
}

inline float dot(const Vector3& a, const Vector3& b)
{
    return a.x()*b.x() + a.y()*b.y() + a.z()*b.z();
}

inline Vector3 cross(const Vector3& a, const Vector3& b)
{
    return Vector3(a.y()*b.z() - a.z()*b.y(),
                   a.z()*b.x() - a.x()*b.z(),
                   a.x()*b.y() - a.y()*b.x());
}

inline float Vector3::length() const
{
    return sqrt(dot(*this, *this));
}

inline Vector3 Vector3::norm() const
{
    return *this / length();
}

#endif // VECTOR3_H
