#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>

class Vector2
{
public:
    Vector2(float x = 0.0f, float y = 0.0f);
    virtual ~Vector2();

    float x() const;
    float y() const;
    float operator[](int index) const;

    void setX(float x);
    void setY(float y);
    void set(float x, float y);

    friend Vector2 operator+(const Vector2& a, const Vector2& b);
    friend Vector2 operator-(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(const Vector2& a, float b);
    friend Vector2 operator*(float a, const Vector2& b);
    friend Vector2 operator/(const Vector2& a, const Vector2& b);
    friend Vector2 operator/(const Vector2& a, float b);

    friend float dot(const Vector2& a, const Vector2& b);

    float length() const;
    Vector2 norm() const;

    friend std::ostream& operator<<(std::ostream& out, const Vector2& vec);

private:
    float _d[2];
};

inline float Vector2::x() const
{
    return _d[0];
}

inline float Vector2::y() const
{
    return _d[1];
}

inline float Vector2::operator[](int index) const
{
    return _d[index];
}

inline void Vector2::setX(float x)
{
    _d[0] = x;
}

inline void Vector2::setY(float y)
{
    _d[1] = y;
}

inline void Vector2::set(float x, float y)
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

inline Vector2 operator*(const Vector2& a, float b)
{
    return Vector2(a.x() * b, a.y() * b);
}

inline Vector2 operator*(float a, const Vector2& b)
{
    return Vector2(a * b.x(), a * b.y());
}

inline Vector2 operator/(const Vector2& a, const Vector2& b)
{
    return Vector2(a.x() / b.x(), a.y() / b.y());
}

inline Vector2 operator/(const Vector2& a, float b)
{
    return Vector2(a.x() / b, a.y() / b);
}

inline float dot(const Vector2& a, const Vector2& b)
{
    return a.x()*b.x() + a.y()*b.y();
}

inline float Vector2::length() const
{
    return sqrt(dot(*this, *this));
}

inline Vector2 Vector2::norm() const
{
    return *this / length();
}

#endif // VECTOR2_H
