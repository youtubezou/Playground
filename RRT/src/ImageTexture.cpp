#include "ImageTexture.h"

ImageTexture::ImageTexture(const char* fileName) :
    _image(nullptr)
{
    _image = new Image();
    _image->readPPM(fileName);
}

ImageTexture::~ImageTexture() noexcept(true)
{
    if (_image != nullptr)
    {
        delete _image;
    }
}

Rgb ImageTexture::value(const Vector2& uv, const Vector3&) const
{
    float u = uv.x() - int(uv.x());
    float v = uv.y() - int(uv.y());
    u *= (_image->width() - 3);
    v *= (_image->height() - 3);

    int iu = (int)u;
    int iv = (int)v;

    float tu = u - iu;
    float tv = v - iv;

    Rgb color = _image->getPixel(iu, iv)*(1-tu)*(1-tv) +
                _image->getPixel(iu+1, iv)*tu*(1-tv) +
                _image->getPixel(iu, iv+1)*(1-tu)*tv+
                _image->getPixel(iu+1, iv+1)*tu*tv;

    return color;
}
