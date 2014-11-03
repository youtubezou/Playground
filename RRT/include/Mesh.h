#ifndef MESH_H
#define MESH_H

#include "Vertex.h"
#include "Shape.h"
#include "Texture.h"

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();
    Mesh(const Mesh&) = default;
    Mesh& operator =(const Mesh&) = default;

    Texture* getTexture() const;

    Vector3* vertex;
    VertexUV* vertexUV;
    VertexN* vertexN;
    VertexUVN* vertexUVN;

private:
    Texture* _texture;
};

#endif // MESH_H
