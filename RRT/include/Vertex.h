#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include "Vector2.h"
#include "Vector3.h"

struct VertexUV
{
    Vector3 vertex;
    Vector2 uv;
};

struct VertexN
{
    Vector3 vertex;
    Vector3 normal;
};

struct VertexUVN
{
    Vector3 vertex;
    Vector3 normal;
    Vector2 uv;
};

#endif // VERTEX_H_INCLUDED
