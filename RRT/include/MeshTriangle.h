#ifndef MESHTRIANGLE_H
#define MESHTRIANGLE_H

#include "Shape.h"
#include "Mesh.h"
#include "Texture.h"

class MeshTriangleUV : public Shape
{
public:
    MeshTriangleUV();
    MeshTriangleUV(Mesh* mesh, int p0, int p1, int p2);
    virtual ~MeshTriangleUV() noexcept(true) = default;

    MeshTriangleUV(const MeshTriangleUV&) = default;
    MeshTriangleUV& operator =(const MeshTriangleUV&) = default;

    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& record) const override;
    virtual bool shadowHit(const Ray& ray, float tmin, float tmax, float time) const override;

private:
    Mesh* _mesh;
    int _p[3];
};

#endif // MESHTRIANGLE_H
