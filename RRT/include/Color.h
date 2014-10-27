#ifndef COLOR_H
#define COLOR_H

#include <iostream>

class Color {
public:
    constexpr Color(double r = 0.0, double g = 0.0, double b = 0.0);

    constexpr double r();
    constexpr double g();
    constexpr double b();
    constexpr double operator[](int index);

    void setR(double r);
    void setG(double g);
    void setB(double b);
    void set(double r, double g, double b);

    friend Color operator+(const Color& a, const Color& b);
    friend Color operator-(const Color& a, const Color& b);
    friend Color operator*(const Color& a, const Color& b);
    friend Color operator*(const Color& a, double b);
    friend Color operator*(double a, const Color& b);
    friend Color operator/(const Color& a, const Color& b);
    friend Color operator/(const Color& a, double b);

    constexpr double clamp(double value);
    void clamp();

    friend std::ostream& operator<<(std::ostream& out, const Color& vec);

private:
    double _d[3];
};

constexpr Color::Color(double r, double g, double b) :
    _d{r, g, b} {
}

inline constexpr double Color::r() {
    return _d[0];
}

inline constexpr double Color::g() {
    return _d[1];
}

inline constexpr double Color::b() {
    return _d[2];
}

inline constexpr double Color::operator[](int index) {
    return _d[index];
}

inline void Color::setR(double r) {
    _d[0] = r;
}

inline void Color::setG(double g) {
    _d[1] = g;
}

inline void Color::setB(double b) {
    _d[2] = b;
}

inline void Color::set(double r, double g, double b) {
    setR(r);
    setG(g);
    setB(b);
}

inline Color operator+(const Color& a, const Color& b) {
    return Color(a.r() + b.r(), a.g() + b.g(), a.b() + b.b());
}

inline Color operator-(const Color& a, const Color& b) {
    return Color(a.r() - b.r(), a.g() - b.g(), a.b() - b.b());
}

inline Color operator*(const Color& a, const Color& b) {
    return Color(a.r() * b.r(), a.g() * b.g(), a.b() * b.b());
}

inline Color operator*(const Color& a, double b) {
    return Color(a.r() * b, a.g() * b, a.b() * b);
}

inline Color operator*(double a, const Color& b) {
    return Color(a * b.r(), a * b.g(), a * b.b());
}

inline Color operator/(const Color& a, const Color& b) {
    return Color(a.r() / b.r(), a.g() / b.g(), a.b() / b.b());
}

inline Color operator/(const Color& a, double b) {
    return Color(a.r() / b, a.g() / b, a.b() / b);
}

inline constexpr double Color::clamp(double value) {
    return (value < 0.0) ? 0.0 : ((value > 1.0 ? 1.0 : value));
}

inline void Color::clamp() {
    setR(clamp(r()));
    setG(clamp(g()));
    setB(clamp(b()));
}

#endif // COLOR_H
