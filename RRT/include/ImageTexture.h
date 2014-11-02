#ifndef IMAGETEXTURE_H
#define IMAGETEXTURE_H

#include "Image.h"
#include "Texture.h"

class ImageTexture : public Texture
{
public:
    ImageTexture(const char* fileName);
    virtual ~ImageTexture() noexcept(true);
    ImageTexture(const ImageTexture&) = default;
    ImageTexture& operator =(const ImageTexture&) = default;

    virtual Rgb value(const Vector2& uv, const Vector3& point) const override;

private:
    Image* _image;
};

#endif // IMAGETEXTURE_H
