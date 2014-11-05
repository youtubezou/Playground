#include "TextureChess.h"

TextureChess::TextureChess(const Color& color1, const Color& color2, double period) :
    _color1(color1), _color2(color2), _period(period)
{
}

TextureChess::~TextureChess()
{
}

Color TextureChess::getColor(const Vector2& uv) const
{
    int x = (int)(uv.x() / _period);
    int y = (int)(uv.y() / _period);
    return ((x + y) & 1) ? _color1 : _color2;
}
