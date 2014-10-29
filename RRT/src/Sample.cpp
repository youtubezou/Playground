#include "Random.h"
#include "Sample.h"

void random(Vector2* samples, int numSamples)
{
    Random random;
    for (int i = 0; i < numSamples; ++i)
    {
        samples[i].setX(random());
        samples[i].setY(random());
    }
}

void jitter(Vector2* samples, int numSamples)
{
    Random random;
    int index = 0;
    int sqrtSamples = (int)sqrt((double)numSamples);
    for (int i = 0; i < sqrtSamples; ++i)
    {
        for (int j = 0; j < sqrtSamples; ++j)
        {
            float x = (i + random()) / sqrtSamples;
            float y = (j + random()) / sqrtSamples;
            samples[index].setX(x);
            samples[index].setY(y);
            ++index;
        }
    }
}

void nrooks(Vector2* samples, int numSamples)
{
    Random random;
    for (int i = 0; i < numSamples; ++i)
    {
        float x = (i + random()) / numSamples;
        float y = (i + random()) / numSamples;
        samples[i].setX(x);
        samples[i].setY(y);
    }
    for (int i = numSamples - 2; i > 0; --i)
    {
        int target = int(random() * i);
        float temp = samples[i + 1].x();
        samples[i + 1].setX(samples[target].x());
        samples[target].setX(temp);
    }
}

void multiJitter(Vector2* samples, int numSamples)
{
    Random random;
    int index = 0;
    int sqrtSamples = (int)sqrt((double)numSamples);
    float cellWidth = 1.0f / numSamples;
    for (int i = 0; i < sqrtSamples; ++i)
    {
        for (int j = 0; j < sqrtSamples; ++j)
        {
            float x = (i * sqrtSamples + j + random()) * cellWidth;
            float y = (j * sqrtSamples + i + random()) * cellWidth;
            samples[index].setX(x);
            samples[index].setY(y);
            ++index;
        }
    }
    for (int i = 0; i < sqrtSamples; ++i)
    {
        for (int j = 0; j < sqrtSamples; ++j)
        {
            int k = j + int(random() * (sqrtSamples - j - i));
            float t = samples[i*sqrtSamples + j].x();
            samples[i*sqrtSamples + j].setX(samples[i*sqrtSamples + k].x());
            samples[i*sqrtSamples + k].setX(t);

            k = j + int(random() * (sqrtSamples - j - i));
            t = samples[j*sqrtSamples + i].y();
            samples[j*sqrtSamples + i].setY(samples[k*sqrtSamples + i].y());
            samples[k*sqrtSamples + i].setY(t);
        }
    }
}


void shuffle(Vector2* samples, int numSamples)
{
    Random random;
    for (int i = numSamples - 2; i > 0; --i)
    {
        int target = int(random() * i);
        float temp = samples[i + 1].x();
        samples[i + 1].setX(samples[target].x());
        samples[target].setX(temp);
    }
}

void boxFilter(Vector2* samples, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        samples[i].setX(samples[i].x() - 0.5f);
        samples[i].setY(samples[i].y() - 0.5f);
    }
}

void tentFilter(Vector2* samples, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        float x = samples[i].x();
        float y = samples[i].y();
        if (x < 0.5f)
        {
            samples[i].setX((float)sqrt(2.0 * x) - 1.0f);
        }
        else
        {
            samples[i].setX(1.0f - (float)sqrt(2.0 - 2.0 * x));
        }
        if (y < 0.5f)
        {
            samples[i].setY((float)sqrt(2.0 * y) - 1.0f);
        }
        else
        {
            samples[i].setY(1.0f - (float)sqrt(2.0 - 2.0 * y));
        }
    }
}

inline float cubicSolve(float r)
{
    float u = r;
    for (int i = 0; i < 5; ++i)
    {
        u = (11.0f * r + u * u * (6.0f + u * (8.0f - 9.0f*u))) /
            (4.0f + 12.0f * u * (1.0f + u * (1.0f - u)));
    }
    return u;
}

inline float cubicFilter(float x)
{
    if (x < 1.0f/24.0f)
    {
        return pow(24 * x, 0.25f) - 2.0f;
    }
    if (x < 0.5)
    {
        return cubicSolve(24.0f * (x - 1.0f/24.0f) / 11.0f);
    }
    if (x < 23.0f/24.0f)
    {
        return 1.0f - cubicSolve(24.0f * (23.0f/24.0f - x) / 11.0f);
    }
    return 2 - pow(24.0f * (1.0f - x), 0.25f);
}

void cubicSplineFilter(Vector2* samples, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        float x = samples[i].x();
        float y = samples[i].y();
        samples[i].setX(cubicFilter(x));
        samples[i].setY(cubicFilter(y));
    }
}

void random(float* samples, int numSamples)
{
    Random random;
    for (int i = 0; i < numSamples; ++i)
    {
        samples[i] = random();
    }
}

void jitter(float* samples, int numSamples)
{
    Random random;
    for (int i = 0; i < numSamples; ++i)
    {
        samples[i] = (i + random()) / numSamples;
    }
}

void shuffle(float* samples, int numSamples)
{
    Random random;
    for (int i = numSamples - 2; i > 0; --i)
    {
        int target = int(random() * i);
        float temp = samples[i + 1];
        samples[i + 1] = samples[target];
        samples[target] = temp;
    }
}
