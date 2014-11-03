#include <cmath>
#include <cstdlib>
#include "Sample.h"

inline float random()
{
    return (float)rand() / (float)RAND_MAX;
}

Sample::Sample(int num, SampleType sampleType, FilterType filterType) :
    _samples(nullptr), _num(num),
    _sampleType(sampleType), _filterType(filterType)
{
    _samples = new Vector2[num];
    resample();
}

Sample::~Sample()
{
    delete[] _samples;
}

Vector2& Sample::operator[](int index)
{
    return _samples[index];
}

void Sample::resample()
{
    switch (_sampleType)
    {
    case SampleType::RANDOM:
        randomSample();
        break;
    case SampleType::JITTERED:
        jitteredSample();
        break;
    }
    switch (_filterType)
    {
    case FilterType::BOX:
        boxFilter();
        break;
    case FilterType::TENT:
        tentFilter();
        break;
    }
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
void Sample::boxFilter()
{
    for (int i = 0; i < _num; ++i)
    {
        _samples[i].set(_samples[i].x() - 0.5f, _samples[i].y() - 0.5f);
    }
}

void Sample::tentFilter()
{
    for (int i = 0; i < _num; ++i)
    {
        float x = _samples[i].x();
        float y = _samples[i].y();
        x = x < 0.5f ? (float)sqrt(2.0 * x) - 1.0f : 1.0f - (float)sqrt(2.0 - 2.0 * x);
        y = y < 0.5f ? (float)sqrt(2.0 * y) - 1.0f : 1.0f - (float)sqrt(2.0 - 2.0 * x);
        _samples[i].set(x, y);
    }
}
