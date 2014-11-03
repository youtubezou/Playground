#ifndef SAMPLE_H
#define SAMPLE_H

#include "Vector2.h"

class Sample
{
public:
    enum Type
    {
        RANDOM,
        JITTERED,
    };
    Sample(int num, Type type);
    virtual ~Sample();

    float x(int index) const;
    float y(int index) const;
    int num() const;
    Vector2& operator[](int index);

private:
    Vector2* _samples;
    int _num;

    void randomSample();
    void jitteredSample();
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
