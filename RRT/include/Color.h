#ifndef COLOR_H
#define COLOR_H

#include <iostream>

class Color
{
public:
    Color(float r = 0.0f, float g = 0.0f, float b = 0.0f);
    virtual ~Color();

    float r() const;
    float g() const;
    float b() const;
    float operator[](int index) const;

    void setR(float r);
    void setG(float g);
    void setB(float b);
    void set(float r, float g, float b);

    friend Color operator+(const Color& a, const Color& b);
    friend Color operator-(const Color& a, const Color& b);
    friend Color operator*(const Color& a, const Color& b);
    friend Color operator*(const Color& a, float b);
    friend Color operator*(float a, const Color& b);
    friend Color operator/(const Color& a, const Color& b);
    friend Color operator/(const Color& a, float b);

    float clamp(float value) const;
    void clamp();

    friend std::ostream& operator<<(std::ostream& out, const Color& vec);

private:
    float _d[3];
};

inline float Color::r() const
{
    return _d[0];
}

inline float Color::g() const
{
    return _d[1];
}

inline float Color::b() const
{
    return _d[2];
}

inline float Color::operator[](int index) const
{
    return _d[index];
}

inline void Color::setR(float r)
{
    _d[0] = r;
}

inline void Color::setG(float g)
{
    _d[1] = g;
}

inline void Color::setB(float b)
{
    _d[2] = b;
}

inline void Color::set(float r, float g, float b)
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

inline Color operator*(const Color& a, float b)
{
    return Color(a.r() + b, a.g() + b, a.b() + b);
}

inline Color operator*(float a, const Color& b)
{
    return Color(a + b.r(), a + b.g(), a + b.b());
}

inline Color operator/(const Color& a, const Color& b)
{
    return Color(a.r() / b.r(), a.g() / b.g(), a.b() / b.b());
}

inline Color operator/(const Color& a, float b)
{
    return Color(a.r() / b, a.g() / b, a.b() / b);
}

inline float Color::clamp(float value) const
{
    if (value < 0.0f)
    {
        return 0.0f;
    }
    if (value > 1.0f)
    {
        return 1.0f;
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
