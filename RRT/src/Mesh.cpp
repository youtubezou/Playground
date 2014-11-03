#include "Mesh.h"

Mesh::Mesh() :
    _texture(nullptr),
    vertex(nullptr), vertexUV(nullptr),
    vertexN(nullptr), vertexUVN(nullptr)
{
}

Mesh::~Mesh()
{
}

Texture* Mesh::getTexture() const
{
    return _texture;
}
