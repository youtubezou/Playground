#include "TextureColor.h"

TextureColor::TextureColor(const Color& color) :
    _color(color)
{
}

TextureColor::~TextureColor()
{
}

Color TextureColor::getColor(const Vector2&) const
{
    return _color;
}
