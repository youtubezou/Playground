#include <cmath>
#include <cstdlib>
#include "Sample.h"

inline float random()
{
    return (float)rand() / (float)RAND_MAX;
}

Sample::Sample(int num, Type type) :
    _samples(nullptr), _num(num)
{
    _samples = new Vector2[num];
    switch (type)
    {
    case RANDOM:
        randomSample();
        break;
    case JITTERED:
        jitteredSample();
        break;
    }
}

Sample::~Sample()
{
    delete[] _samples;
}

Vector2& Sample::operator[](int index)
{
    return _samples[index];
}

void Sample::randomSample()
{
    for (int i = 0; i < _num; ++i)
    {
        _samples[i].set(random(), random());
    }
}

void Sample::jitteredSample()
{
    int index = 0;
    int sqrtNum = (int)sqrt(_num);
    for (int i = 0; i < sqrtNum; ++i)
    {
        for (int j = 0; j < sqrtNum; ++j)
        {
            float x = (i + random()) / sqrtNum;
            float y = (j + random()) / sqrtNum;
            _samples[index++].set(x, y);
        }
    }
}
