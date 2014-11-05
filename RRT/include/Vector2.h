#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>

class Vector2
{
public:
    Vector2(double x = 0.0, double y = 0.0);
    virtual ~Vector2();

    double x() const;
    double y() const;
    double operator[](int index) const;

    void setX(double x);
    void setY(double y);
    void set(double x, double y);

    friend Vector2 operator+(const Vector2& a, const Vector2& b);
    friend Vector2 operator-(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(const Vector2& a, double b);
    friend Vector2 operator*(double a, const Vector2& b);
    friend Vector2 operator/(const Vector2& a, const Vector2& b);
    friend Vector2 operator/(const Vector2& a, double b);

    friend double dot(const Vector2& a, const Vector2& b);

    double length() const;
    Vector2 norm() const;

    friend std::ostream& operator<<(std::ostream& out, const Vector2& vec);

private:
    double _d[2];
};

inline double Vector2::x() const
{
    return _d[0];
}

inline double Vector2::y() const
{
    return _d[1];
}

inline double Vector2::operator[](int index) const
{
    return _d[index];
}

inline void Vector2::setX(double x)
{
    _d[0] = x;
}

inline void Vector2::setY(double y)
{
    _d[1] = y;
}

inline void Vector2::set(double x, double y)
{
    setX(x);
    setY(y);
}

inline Vector2 operator+(const Vector2& a, const Vector2& b)
{
    return Vector2(a.x() + b.x(), a.y() + b.y());
}

inline Vector2 operator-(const Vector2& a, const Vector2& b)
{
    return Vector2(a.x() - b.x(), a.y() - b.y());
}

inline Vector2 operator*(const Vector2& a, const Vector2& b)
{
    return Vector2(a.x() * b.x(), a.y() * b.y());
}

inline Vector2 operator*(const Vector2& a, double b)
{
    return Vector2(a.x() * b, a.y() * b);
}

inline Vector2 operator*(double a, const Vector2& b)
{
    return Vector2(a * b.x(), a * b.y());
}

inline Vector2 operator/(const Vector2& a, const Vector2& b)
{
    return Vector2(a.x() / b.x(), a.y() / b.y());
}

inline Vector2 operator/(const Vector2& a, double b)
{
    return Vector2(a.x() / b, a.y() / b);
}

inline double dot(const Vector2& a, const Vector2& b)
{
    return a.x()*b.x() + a.y()*b.y();
}

inline double Vector2::length() const
{
    return sqrt(dot(*this, *this));
}

inline Vector2 Vector2::norm() const
{
    return *this / length();
}

#endif // VECTOR2_H
