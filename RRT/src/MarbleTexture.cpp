#include "MarbleTexture.h"
#define M_PI 3.14159265358979323846

MarbleTexture::MarbleTexture(float stripesPerUnit, float scale, int octaves) :
    _freq(0.0f), _scale(scale),
    _octaves(octaves),
    _c0(0.8, 0.8, 0.8), _c1(0.5, 0.2, 0.1), _c2(0.06, 0.04, 0.04),
    _noise()
{
    _freq = M_PI * stripesPerUnit;
}

MarbleTexture::MarbleTexture(const Rgb& c0, const Rgb& c1, const Rgb& c2, float stripesPerUnit, float scale, int octaves) :
    _freq(0.0f), _scale(scale),
    _octaves(octaves),
    _c0(c0), _c1(c1), _c2(c2),
    _noise()
{
    _freq = M_PI * stripesPerUnit;
}

Rgb MarbleTexture::value(const Vector2& uv, const Vector3& point) const
{
    float temp = _scale * _noise.turbulence(_freq * point, _octaves);
    float t = 2.0f * fabs(sin(_freq*point.x() + temp));

    if (t < 1.0f)
    {
        return _c1*t + (1.0f - t)*_c2;
    }
    t -= 1.0f;
    return _c0*t + (1.0f - t)*_c1;
}
