#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3
{
public:
    Vector3(double x = 0.0, double y = 0.0, double z = 0.0);
    virtual ~Vector3();

    double x() const;
    double y() const;
    double z() const;
    double operator[](int index) const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void set(double x, double y, double z);

    friend Vector3 operator+(const Vector3& a, const Vector3& b);
    friend Vector3 operator-(const Vector3& a, const Vector3& b);
    friend Vector3 operator-(const Vector3& a);
    friend Vector3 operator*(const Vector3& a, const Vector3& b);
    friend Vector3 operator*(const Vector3& a, double b);
    friend Vector3 operator*(double a, const Vector3& b);
    friend Vector3 operator/(const Vector3& a, const Vector3& b);
    friend Vector3 operator/(const Vector3& a, double b);

    friend double dot(const Vector3& a, const Vector3& b);
    friend Vector3 cross(const Vector3& a, const Vector3& b);

    double length() const;
    Vector3 norm() const;

    friend std::ostream& operator<<(std::ostream& out, const Vector3& vec);

private:
    double _d[3];
};

inline double Vector3::x() const
{
    return _d[0];
}

inline double Vector3::y() const
{
    return _d[1];
}

inline double Vector3::z() const
{
    return _d[2];
}

inline double Vector3::operator[](int index) const
{
    return _d[index];
}

inline void Vector3::setX(double x)
{
    _d[0] = x;
}

inline void Vector3::setY(double y)
{
    _d[1] = y;
}

inline void Vector3::setZ(double z)
{
    _d[2] = z;
}

inline void Vector3::set(double x, double y, double z)
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

inline Vector3 operator*(const Vector3& a, double b)
{
    return Vector3(a.x() * b, a.y() * b, a.z() * b);
}

inline Vector3 operator*(double a, const Vector3& b)
{
    return Vector3(a * b.x(), a * b.y(), a * b.z());
}

inline Vector3 operator/(const Vector3& a, const Vector3& b)
{
    return Vector3(a.x() / b.x(), a.y() / b.y(), a.z() / b.z());
}

inline Vector3 operator/(const Vector3& a, double b)
{
    return Vector3(a.x() / b, a.y() / b, a.z() / b);
}

inline double dot(const Vector3& a, const Vector3& b)
{
    return a.x()*b.x() + a.y()*b.y() + a.z()*b.z();
}

inline Vector3 cross(const Vector3& a, const Vector3& b)
{
    return Vector3(a.y()*b.z() - a.z()*b.y(),
                   a.z()*b.x() - a.x()*b.z(),
                   a.x()*b.y() - a.y()*b.x());
}

inline double Vector3::length() const
{
    return sqrt(dot(*this, *this));
}

inline Vector3 Vector3::norm() const
{
    return *this / length();
}

#endif // VECTOR3_H
