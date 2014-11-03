#ifndef FILTER_H
#define FILTER_H

#include "Sample.h"

class Filter
{
public:
    enum Type
    {
        BOX,
        TENT,
    };

    Filter();
    virtual ~Filter();

    void filter(Sample& sample, Type type);

private:
    void boxFilter(Sample& sample);
    void tentFilter(Sample& sample);
};

#endif // FILTER_H
