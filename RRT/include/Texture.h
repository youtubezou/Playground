#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "Vector2.h"
#include "Vector3.h"
#include "Rgb.h"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual Rgb value(const Vector2& uv, const Vector3& point) const = 0;
};

#endif // TEXTURE_H_INCLUDED
