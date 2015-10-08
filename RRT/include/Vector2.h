#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>

class Vector2 {
public:
    constexpr Vector2(double x = 0.0, double y = 0.0);

    constexpr double x();
    constexpr double y();
    constexpr double operator[](int index);

    void setX(double x);
    void setY(double y);
    void set(double x, double y);

    friend constexpr Vector2 operator+(const Vector2& a, const Vector2& b);
    friend constexpr Vector2 operator-(const Vector2& a, const Vector2& b);
    friend constexpr Vector2 operator*(const Vector2& a, const Vector2& b);
    friend constexpr Vector2 operator*(const Vector2& a, double b);
    friend constexpr Vector2 operator*(double a, const Vector2& b);
    friend constexpr Vector2 operator/(const Vector2& a, const Vector2& b);
    friend constexpr Vector2 operator/(const Vector2& a, double b);

    friend constexpr double dot(const Vector2& a, const Vector2& b);

    double constexpr length();
    Vector2 constexpr norm();

    friend std::ostream& operator<<(std::ostream& out, const Vector2& vec);

private:
    double _d[2];
};

constexpr Vector2::Vector2(double x, double y) : _d{x, y} {
}

inline constexpr double Vector2::x() {
    return _d[0];
}

inline constexpr double Vector2::y() {
    return _d[1];
}

inline constexpr double Vector2::operator[](int index) {
    return _d[index];
}

inline void Vector2::setX(double x) {
    _d[0] = x;
}

inline void Vector2::setY(double y) {
    _d[1] = y;
}

inline void Vector2::set(double x, double y) {
    setX(x);
    setY(y);
}

inline constexpr Vector2 operator+(const Vector2& a, const Vector2& b) {
    return Vector2(a.x() + b.x(), a.y() + b.y());
}

inline constexpr Vector2 operator-(const Vector2& a, const Vector2& b) {
    return Vector2(a.x() - b.x(), a.y() - b.y());
}

inline constexpr Vector2 operator*(const Vector2& a, const Vector2& b) {
    return Vector2(a.x() * b.x(), a.y() * b.y());
}

inline constexpr Vector2 operator*(const Vector2& a, double b) {
    return Vector2(a.x() * b, a.y() * b);
}

inline constexpr Vector2 operator*(double a, const Vector2& b) {
    return Vector2(a * b.x(), a * b.y());
}

inline constexpr Vector2 operator/(const Vector2& a, const Vector2& b) {
    return Vector2(a.x() / b.x(), a.y() / b.y());
}

inline constexpr Vector2 operator/(const Vector2& a, double b) {
    return Vector2(a.x() / b, a.y() / b);
}

inline constexpr double dot(const Vector2& a, const Vector2& b) {
    return a.x()*b.x() + a.y()*b.y();
}

inline constexpr double Vector2::length() {
    return sqrt(dot(*this, *this));
}

inline constexpr Vector2 Vector2::norm() {
    return *this / length();
}

#endif // VECTOR2_H
