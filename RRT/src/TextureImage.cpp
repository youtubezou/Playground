#include "TextureImage.h"

TextureImage::TextureImage(const char* fileName) :
    _image() {
    _image.readPPM(fileName);
}

Color TextureImage::getColor(const Vector2& uv) const {
    int x = uv.x() * _image.w();
    int y = uv.y() * _image.h();
    return _image.get(x, y);
}
