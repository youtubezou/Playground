#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

Color Texture::getColor(const Vector2& uv) const
{
    return Color(1.0, 1.0, 1.0);
}
