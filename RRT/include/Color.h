#ifndef COLOR_H
#define COLOR_H

#include <iostream>

class Color
{
public:
    Color(double r = 0.0, double g = 0.0, double b = 0.0);
    virtual ~Color();

    double r() const;
    double g() const;
    double b() const;
    double operator[](int index) const;

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

    double clamp(double value) const;
    void clamp();

    friend std::ostream& operator<<(std::ostream& out, const Color& vec);

private:
    double _d[3];
};

inline double Color::r() const
{
    return _d[0];
}

inline double Color::g() const
{
    return _d[1];
}

inline double Color::b() const
{
    return _d[2];
}

inline double Color::operator[](int index) const
{
    return _d[index];
}

inline void Color::setR(double r)
{
    _d[0] = r;
}

inline void Color::setG(double g)
{
    _d[1] = g;
}

inline void Color::setB(double b)
{
    _d[2] = b;
}

inline void Color::set(double r, double g, double b)
{
    setR(r);
    setG(g);
    setB(b);
}

inline Color operator+(const Color& a, const Color& b)
{
    return Color(a.r() + b.r(), a.g() + b.g(), a.b() + b.b());
}

inline Color operator-(const Color& a, const Color& b)
{
    return Color(a.r() - b.r(), a.g() - b.g(), a.b() - b.b());
}

inline Color operator*(const Color& a, const Color& b)
{
    return Color(a.r() * b.r(), a.g() * b.g(), a.b() * b.b());
}

inline Color operator*(const Color& a, double b)
{
    return Color(a.r() + b, a.g() + b, a.b() + b);
}

inline Color operator*(double a, const Color& b)
{
    return Color(a + b.r(), a + b.g(), a + b.b());
}

inline Color operator/(const Color& a, const Color& b)
{
    return Color(a.r() / b.r(), a.g() / b.g(), a.b() / b.b());
}

inline Color operator/(const Color& a, double b)
{
    return Color(a.r() / b, a.g() / b, a.b() / b);
}

inline double Color::clamp(double value) const
{
    if (value < 0.0)
    {
        return 0.0;
    }
    if (value > 1.0)
    {
        return 1.0;
    }
    return value;
}

inline void Color::clamp()
{
    setR(clamp(r()));
    setG(clamp(g()));
    setB(clamp(b()));
}

#endif // COLOR_H
