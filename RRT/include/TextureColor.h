#ifndef TEXTURECOLOR_H
#define TEXTURECOLOR_H

#include "Texture.h"

class TextureColor : public Texture {
public:
    TextureColor(const Color& color);

    virtual Color getColor(const Vector2& uv) const override final;

private:
    Color _color;
};

#endif // TEXTURECOLOR_H
