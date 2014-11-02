#include "SimpleTexture.h"

SimpleTexture::SimpleTexture(Rgb color) :
    _color(color)
{
}

Rgb SimpleTexture::value(const Vector2&, const Vector3&) const
{
    return _color;
}
