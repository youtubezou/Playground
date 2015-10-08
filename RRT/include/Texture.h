#ifndef TEXTURE_H
#define TEXTURE_H

#include "Vector2.h"
#include "Color.h"

class Texture {
public:
    Texture();
    virtual ~Texture();

    virtual Color getColor(const Vector2& uv) const;
};

#endif // TEXTURE_H
