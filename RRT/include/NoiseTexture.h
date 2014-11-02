#ifndef NOISETEXTURE_H
#define NOISETEXTURE_H

#include <cmath>
#include "Texture.h"
#include "Rgb.h"
#include "SolidNoise.h"

class NoiseTexture : public Texture
{
public:
    NoiseTexture(float scale);
    NoiseTexture(const Rgb& c0, const Rgb& c1, float scale = 1.0f);

    virtual Rgb value(const Vector2& uv, const Vector3& point) const override;

private:
    float _scale;
    Rgb _c0, _c1;
    SolidNoise _noise;
};

#endif // NOISETEXTURE_H
