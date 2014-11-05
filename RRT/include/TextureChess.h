#ifndef TEXTURECHESS_H
#define TEXTURECHESS_H

#include "Texture.h"

class TextureChess : public Texture
{
public:
    TextureChess(const Color& color1, const Color& color2, double period);
    virtual ~TextureChess();

    virtual Color getColor(const Vector2& uv) const override final;

private:
    Color _color1, _color2;
    double _period;
};

#endif // TEXTURECHESS_H
