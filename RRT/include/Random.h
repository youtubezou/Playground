#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

class Random
{
public:
    Random(unsigned long long seed = 7564231ULL);

    float operator ()();

private:
    unsigned long long _seed;
    unsigned long long _mult;
    unsigned long long _lmax;
    float _fmax;
};

Random::Random(unsigned long long seed) :
    _seed(seed), _mult(62089911ULL),
    _lmax(4294967295ULL),
    _fmax(4294967295.0f)
{
}

float Random::operator ()()
{
    _seed = _mult * _seed;
    return float(_seed % _lmax) / _fmax;
}

#endif // RANDOM_H_INCLUDED
