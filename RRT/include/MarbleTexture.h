#ifndef MARBLETEXTURE_H
#define MARBLETEXTURE_H

#include <cmath>
#include "SolidNoise.h"
#include "Texture.h"

class MarbleTexture : public Texture
{
public:
    MarbleTexture(float stripesPerUnit, float scale = 5.0f, int octaves = 8);
    MarbleTexture(const Rgb& c0, const Rgb& c1, const Rgb& c2, float stripesPerUnit, float scale = 3.0f, int octaves = 8);

    virtual Rgb value(const Vector2& uv, const Vector3& point) const override;

private:
    float _freq, _scale;
    int _octaves;
    Rgb _c0, _c1, _c2;
    SolidNoise _noise;
};

#endif // MARBLETEXTURE_H
