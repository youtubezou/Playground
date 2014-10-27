#include "TextureColor.h"

TextureColor::TextureColor(const Color& color) :
    _color(color) {
}

Color TextureColor::getColor(const Vector2&) const {
    return _color;
}
