#include "MeshTriangle.h"

MeshTriangleUV::MeshTriangleUV() :
    _mesh(nullptr)
{
}

MeshTriangleUV::MeshTriangleUV(Mesh* mesh, int p0, int p1, int p2) :
    _mesh(mesh)
{
    _p[0] = p0;
    _p[1] = p1;
    _p[2] = p2;
}

bool MeshTriangleUV::hit(const Ray& ray, float tmin, float tmax, float, HitRecord& record) const
{
    Vector3 _a(_mesh->vertexUV[_p[0]].vertex);
    Vector3 _b(_mesh->vertexUV[_p[1]].vertex);
    Vector3 _c(_mesh->vertexUV[_p[2]].vertex);

    float a = _a.x() - _b.x();
    float b = _a.y() - _b.y();
    float c = _a.z() - _b.z();

    float d = _a.x() - _c.x();
    float e = _a.y() - _c.y();
    float f = _a.z() - _c.z();

    float g = ray.direction().x();
    float h = ray.direction().y();
    float i = ray.direction().z();

    float j = _a.x() - ray.origin().x();
    float k = _a.y() - ray.origin().y();
    float l = _a.z() - ray.origin().z();

    float ei_hf = e*i - h*f;
    float gf_di = g*f - d*i;
    float dh_eg = d*h - e*g;

    float M = a*ei_hf + b*gf_di + c*dh_eg;

    float beta = (j*ei_hf + k*gf_di + l*dh_eg) / M;
    if (beta <= 0.0f || 1.0f <= beta)
    {
        return false;
    }

    float ak_jb = a*k - j*b;
    float jc_al = j*c - a*l;
    float bl_kc = b*l - k*c;

    float gamma = (i*ak_jb + h*jc_al + g*bl_kc) / M;
    if (gamma <= 0.0f || 1.0f <= beta + gamma)
    {
        return false;
    }

    float t = -(f*ak_jb + e*jc_al + d*bl_kc) / M;
    if (t < tmin || tmax < t)
    {
        return false;
    }
    record.t = t;
    record.normal = unitVector(cross(_b - _a, _c - _a));

    double alpha = 1.0 - beta - gamma;
    Vector2 u0(_mesh->vertexUV[_p[0]].uv);
    Vector2 u1(_mesh->vertexUV[_p[1]].uv);
    Vector2 u2(_mesh->vertexUV[_p[2]].uv);

    record.uv = Vector2(alpha*u0.x() + beta*u1.x() + gamma*u2.x(),
                        alpha*u0.y() + beta*u1.y() + gamma*u2.y());
    record.texture = _mesh->getTexture();
    return true;
}

bool MeshTriangleUV::shadowHit(const Ray& ray, float tmin, float tmax, float) const
{
    Vector3 _a(_mesh->vertexUV[_p[0]].vertex);
    Vector3 _b(_mesh->vertexUV[_p[1]].vertex);
    Vector3 _c(_mesh->vertexUV[_p[2]].vertex);

    float a = _a.x() - _b.x();
    float b = _a.y() - _b.y();
    float c = _a.z() - _b.z();

    float d = _a.x() - _c.x();
    float e = _a.y() - _c.y();
    float f = _a.z() - _c.z();

    float g = ray.direction().x();
    float h = ray.direction().y();
    float i = ray.direction().z();

    float j = _a.x() - ray.origin().x();
    float k = _a.y() - ray.origin().y();
    float l = _a.z() - ray.origin().z();

    float ei_hf = e*i - h*f;
    float gf_di = g*f - d*i;
    float dh_eg = d*h - e*g;

    float M = a*ei_hf + b*gf_di + c*dh_eg;

    float beta = (j*ei_hf + k*gf_di + l*dh_eg) / M;
    if (beta <= 0.0f || 1.0f <= beta)
    {
        return false;
    }

    float ak_jb = a*k - j*b;
    float jc_al = j*c - a*l;
    float bl_kc = b*l - k*c;

    float gamma = (i*ak_jb + h*jc_al + g*bl_kc) / M;
    if (gamma <= 0.0f || 1.0f <= beta + gamma)
    {
        return false;
    }

    float t = -(f*ak_jb + e*jc_al + d*bl_kc) / M;
    return tmin <= t && t <= tmax;
}
