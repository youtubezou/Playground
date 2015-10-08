#ifndef RADIANCE_H
#define RADIANCE_H

#include "Shape.h"

class Radiance {
public:
    Radiance(Shape** shapes, int n, const Color& background);
    virtual ~Radiance();

    Color radiance(const Ray& ray, int depth, bool emit = true);
    Color idealDiffuse(const Ray& ray, const HitRecord& record, Shape* shape, const Color& color, int depth, bool emit);
    Color idealSpecular(const Ray& ray, const HitRecord& record, Shape* shape, const Color& color, int depth);
    Color idealRefraction(const Ray& ray, const HitRecord& record, Shape* shape, const Color& color, int depth);

private:
    Shape** _shapes;
    int _shapeNum;
    Color _background;
};

#endif // RADIANCE_H
