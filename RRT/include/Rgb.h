#ifndef Rgb_H
#define Rgb_H

#include <iostream>

class Rgb
{
public:
    Rgb();
    Rgb(float red, float green, float blue);
    Rgb(const Rgb& rgb);

    float r() const;
    float g() const;
    float b() const;

    void setRed(float red);
    void setGreen(float green);
    void setBlue(float blue);

    Rgb& operator =(const Rgb& right);

    Rgb operator +() const;
    Rgb operator -() const;
    friend Rgb operator *(float f, const Rgb& c);
    friend Rgb operator *(const Rgb& c, float f);
    friend Rgb operator /(const Rgb& c, float f);
    friend Rgb operator +(const Rgb& c1, const Rgb& c2);
    friend Rgb operator -(const Rgb& c1, const Rgb& c2);
    friend Rgb operator *(const Rgb& c1, const Rgb& c2);
    friend Rgb operator /(const Rgb& c1, const Rgb& c2);
    Rgb& operator +=(const Rgb& right);
    Rgb& operator -=(const Rgb& right);
    Rgb& operator *=(const Rgb& right);
    Rgb& operator *=(float right);
    Rgb& operator /=(const Rgb& right);
    Rgb& operator /=(float right);

    friend std::ostream& operator <<(std::ostream& out, const Rgb& rgb);

    void clamp();

private:
    float _r;
    float _g;
    float _b;
};

inline Rgb::Rgb() :
    _r(0.0f), _g(0.0f), _b(0.0f)
{
}

inline Rgb::Rgb(float red, float green, float blue) :
    _r(red), _g(green), _b(blue)
{
}

inline Rgb::Rgb(const Rgb& rgb)
{
    *this = rgb;
}

inline float Rgb::r() const
{
    return _r;
}

inline float Rgb::g() const
{
    return _g;
}

inline float Rgb::b() const
{
    return _b;
}

inline void Rgb::setRed(float red)
{
    _r = red;
}

inline void Rgb::setGreen(float green)
{
    _g = green;
}

inline void Rgb::setBlue(float blue)
{
    _b = blue;
}

inline Rgb& Rgb::operator =(const Rgb& right)
{
    _r = right._r;
    _g = right._g;
    _b = right._b;
    return *this;
}

inline Rgb Rgb::operator +() const
{
    return *this;
}

inline Rgb Rgb::operator -() const
{
    return Rgb(-_r, -_g, -_b);
}

inline Rgb operator *(float f, const Rgb& c)
{
    return Rgb(c._r * f, c._g * f, c._b * f);
}

inline Rgb operator *(const Rgb& c, float f)
{
    return Rgb(c._r * f, c._g * f, c._b * f);
}

inline Rgb operator /(const Rgb& c, float f)
{
    return Rgb(c._r / f, c._g * f, c._b / f);
}

inline Rgb operator +(const Rgb& c1, const Rgb& c2)
{
    return Rgb(c1._r + c2._r, c1._g + c2._g, c1._b + c2._b);
}

inline Rgb operator -(const Rgb& c1, const Rgb& c2)
{
    return Rgb(c1._r - c2._r, c1._g - c2._g, c1._b - c2._b);
}

inline Rgb operator *(const Rgb& c1, const Rgb& c2)
{
    return Rgb(c1._r * c2._r, c1._g * c2._g, c1._b * c2._b);
}

inline Rgb operator /(const Rgb& c1, const Rgb& c2)
{
    return Rgb(c1._r / c2._r, c1._g / c2._g, c1._b / c2._b);
}

inline Rgb& Rgb::operator +=(const Rgb& right)
{
    *this = *this + right;
    return *this;
}

inline Rgb& Rgb::operator -=(const Rgb& right)
{
    *this = *this - right;
    return *this;
}

inline Rgb& Rgb::operator *=(const Rgb& right)
{
    *this = *this * right;
    return *this;
}

inline Rgb& Rgb::operator *=(float right)
{
    *this = *this * right;
    return *this;
}

inline Rgb& Rgb::operator /=(const Rgb& right)
{
    *this = *this / right;
    return *this;
}

inline Rgb& Rgb::operator /=(float right)
{
    *this = *this / right;
    return *this;
}

inline std::ostream& operator <<(std::ostream& out, const Rgb& rgb)
{
    out << rgb._r << ' ' << rgb._g << ' ' << rgb._b << ' ';
    return out;
}

inline void Rgb::clamp()
{
    if (_r > 1.0f)
    {
        _r = 1.0f;
    }
    else if (_r < 0.0f)
    {
        _r = 0.0f;
    }
    if (_g > 1.0f)
    {
        _g = 1.0f;
    }
    else if (_g < 0.0f)
    {
        _g = 0.0f;
    }
    if (_b > 1.0f)
    {
        _b = 1.0f;
    }
    else if (_b < 0.0f)
    {
        _b = 0.0f;
    }
}

#endif // Rgb_H
