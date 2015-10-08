#ifndef TEXTUREIMAGE_H
#define TEXTUREIMAGE_H

#include "Texture.h"
#include "Image.h"

class TextureImage : public Texture {
public:
    TextureImage(const char* fileName);

    virtual Color getColor(const Vector2& uv) const override final;

private:
    Image _image;
};

#endif // TEXTUREIMAGE_H
