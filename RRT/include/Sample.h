#ifndef SAMPLE_H
#define SAMPLE_H

#include "Vector2.h"

class Sample
{
public:
    enum class SampleType
    {
        RANDOM,
        JITTERED,
    };
    enum class FilterType
    {
        BOX,
        TENT,
    };
    Sample(int num, SampleType sampleType, FilterType filterType);
    virtual ~Sample();

    float x(int index) const;
    float y(int index) const;
    int num() const;
    Vector2& operator[](int index);

    void resample();

private:
    Vector2* _samples;
    int _num;
    SampleType _sampleType;
    FilterType _filterType;

    void randomSample();
    void jitteredSample();

    void boxFilter();
    void tentFilter();
};

inline float Sample::x(int index) const
{
    return _samples[index].x();
}

inline float Sample::y(int index) const
{
    return _samples[index].y();
}

inline int Sample::num() const
{
    return _num;
}

#endif // SAMPLE_H
