#include "Random.h"
#include "SolidNoise.h"

SolidNoise::SolidNoise()
{
    Random random;

    _grad[0] = Vector3( 1, 1, 0);
    _grad[1] = Vector3(-1, 1, 0);
    _grad[2] = Vector3( 1,-1, 0);
    _grad[3] = Vector3(-1,-1, 0);

    _grad[4] = Vector3( 1, 0, 1);
    _grad[5] = Vector3(-1, 0, 1);
    _grad[6] = Vector3( 1, 0,-1);
    _grad[7] = Vector3(-1, 0,-1);

    _grad[8] = Vector3( 0, 1, 1);
    _grad[9] = Vector3( 0,-1, 1);
    _grad[10]= Vector3( 0, 1,-1);
    _grad[11]= Vector3( 0,-1,-1);

    _grad[12]= Vector3( 1, 1, 0);
    _grad[13]= Vector3(-1, 1, 0);
    _grad[14]= Vector3( 0,-1, 1);
    _grad[15]= Vector3( 0,-1,-1);

    for (int i = 0; i < 16; ++i)
    {
        _phi[i] = i;
    }

    for (int i = 14; i >= 0; --i)
    {
        int target = int(random() * i);
        int temp = _phi[i + 1];
        _phi[i + 1] = _phi[target];
        _phi[target] = temp;
    }
}

float SolidNoise::turbulence(const Vector3& p, int depth) const
{
    float sum = 0.0f;
    float weight = 1.0f;
    Vector3 temp(p);

    sum = fabs(noise(temp));

    for (int i = 1; i < depth; ++i)
    {
        weight = weight * 2;
        temp.setX(p.x() * weight);
        temp.setY(p.y() * weight);
        temp.setZ(p.z() * weight);
        sum += fabs(noise(temp)) / weight;
    }
    return sum;
}

float SolidNoise::dturbulence(const Vector3& p, int depth, float d) const
{
    float sum = 0.0f;
    float weight = 1.0f;
    Vector3 temp(p);

    sum = fabs(noise(temp)) / d;

    for (int i = 1; i < depth; ++i)
    {
        weight = weight * d;
        temp.setX(p.x() * weight);
        temp.setY(p.y() * weight);
        temp.setZ(p.z() * weight);
        sum += fabs(noise(temp)) / d;
    }
    return sum;
}

float SolidNoise::noise(const Vector3& p) const
{
    int fi, fj, fk;
    float fu, fv, fw, sum;
    Vector3 v;
    fi = int(floor(p.x()));
    fj = int(floor(p.y()));
    fk = int(floor(p.z()));
    fu = p.x() - float(fi);
    fv = p.y() - float(fj);
    fw = p.z() - float(fk);
    sum = 0.0;

    v = Vector3(fu, fv, fw);
    sum += knot(fi, fj, fk, v);

    v = Vector3(fu - 1, fv, fw);
    sum += knot(fi + 1, fj, fk, v);

    v = Vector3(fu, fv - 1, fw);
    sum += knot(fi, fj + 1, fk, v);

    v = Vector3(fu - 1, fv - 1, fw);
    sum += knot(fi + 1, fj + 1, fk, v);

    v = Vector3(fu - 1, fv, fw - 1);
    sum += knot(fi + 1, fj, fk + 1, v);

    v = Vector3(fu, fv - 1, fw - 1);
    sum += knot(fi, fj + 1, fk + 1, v);

    v = Vector3(fu - 1, fv - 1, fw - 1);
    sum += knot(fi + 1, fj + 1, fk + 1, v);

    return sum;
}
