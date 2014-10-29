#ifndef SAMPLE_H
#define SAMPLE_H

#include "Vector2.h"

void random(Vector2* samples, int numSamples);
void jitter(Vector2* samples, int numSamples);
void nrooks(Vector2* samples, int numSamples);
void multiJitter(Vector2* samples, int numSamples);
void shuffle(Vector2* samples, int numSamples);

void boxFilter(Vector2* samples, int numSamples);
void tentFilter(Vector2* samples, int numSamples);
void cubicSplineFilter(Vector2* samples, int numSamples);

void random(float* samples, int numSamples);
void jitter(float* samples, int numSamples);
void shuffle(float* samples, int numSamples);

#endif // SAMPLE_H
