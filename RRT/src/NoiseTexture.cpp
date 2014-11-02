#include "NoiseTexture.h"

NoiseTexture::NoiseTexture(float scale) :
        _c0(), _c1(), _scale(scale)
{
}

NoiseTexture::NoiseTexture(const Rgb& c0, const Rgb& c1, float scale) :
    _c0(c0), _c1(c1), _scale(scale)
{
}

Rgb NoiseTexture::value(const Vector2&, const Vector3& point) const
{
    float t = (1.0f + _noise.noise(point * _scale)) * 0.5f;
    return t*_c0 + (1.0f - t)*_c1;
}
