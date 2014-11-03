#include "Filter.h"

Filter::Filter()
{
}

Filter::~Filter()
{
}

void Filter::filter(Sample& sample, Type type)
{
    switch (type)
    {
    case BOX:
        boxFilter(sample);
        break;
    case TENT:
        tentFilter(sample);
        break;
    }
}

void Filter::boxFilter(Sample& sample)
{
    for (int i = 0; i < sample.num(); ++i)
    {
        sample[i].set(sample.x(i) - 0.5f, sample.y(i) - 0.5f);
    }
}

void Filter::tentFilter(Sample& sample)
{
    for (int i = 0; i < sample.num(); ++i)
    {
        float x = sample.x(i);
        float y = sample.y(i);
        x = x < 0.5f ? (float)sqrt(2.0 * x) - 1.0f : 1.0f - (float)sqrt(2.0 - 2.0 * x);
        y = y < 0.5f ? (float)sqrt(2.0 * y) - 1.0f : 1.0f - (float)sqrt(2.0 - 2.0 * x);
        sample[i].set(x, y);
    }
}
