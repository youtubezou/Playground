#ifndef SOLIDNOISE_H
#define SOLIDNOISE_H

#include <cmath>
#include <algorithm>
#include "Vector3.h"

class SolidNoise
{
public:
    SolidNoise();
    float noise(const Vector3&) const;
    float turbulence(const Vector3& p, int depth) const;
    float dturbulence(const Vector3& p, int depth, float d) const;
    float omega(float t) const;
    Vector3 gamma(int i, int j, int k) const;
    int intGamma(int i, int j) const;
    float knot(int i, int j, int k, Vector3& v) const;

private:
    Vector3 _grad[16];
    int _phi[16];
};

inline float SolidNoise::omega(float t) const
{
    t = (t > 0.0f) ? t : -t;
    return (-6.0f*t*t*t*t*t + 15.0f*t*t*t*t - 10.0f*t*t*t + 1.0f);
}

inline Vector3 SolidNoise::gamma(int i, int j, int k) const
{
    int index;
    index = _phi[std::abs(k) % 16];
    index = _phi[std::abs(j + index) % 16];
    index = _phi[std::abs(i + index) % 16];
    return _grad[index];
}

inline float SolidNoise::knot(int i, int j, int k, Vector3& v) const
{
    return (omega(v.x()) * omega(v.y()) * omega(v.z()) * (dot(gamma(i, j, k), v)));
}

inline int SolidNoise::intGamma(int i, int j) const
{
    int index;
    index = _phi[std::abs(j) % 16];
    index = _phi[std::abs(i + index) % 16];
    return index;
}

#endif // SOLIDNOISE_H
