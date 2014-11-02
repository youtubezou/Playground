#ifndef SIMPLETEXTURE_H
#define SIMPLETEXTURE_H

#include "Texture.h"

class SimpleTexture : public Texture
{
public:
    SimpleTexture(Rgb color);

    virtual Rgb value(const Vector2& uv, const Vector3& point) const override;

private:
    Rgb _color;
};

#endif // SIMPLETEXTURE_H
