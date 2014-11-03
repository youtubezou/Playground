#include "Shape.h"

Shape::Shape() :
    _texture(nullptr),
    _material(Material::DIFFUSE),
    _emission(0.0f, 0.0f, 0.0f),
    _dielectric(1.0f)
{
}

Shape::~Shape()
{
}

Texture* Shape::texture() const
{
    return _texture;
}

void Shape::setTexture(Texture* texture)
{
    _texture = texture;
}

Material Shape::material() const
{
    return _material;
}

void Shape::setMaterial(Material material)
{
    _material = material;
}

const Color& Shape::emission() const
{
    return _emission;
}

void Shape::setEmission(const Color& color)
{
    _emission = color;
}

float Shape::dielectric() const
{
    return _dielectric;
}

void Shape::setDielectric(float dielectric)
{
    _dielectric = dielectric;
}

bool Shape::hitTest(const Ray&, HitRecord&) const
{
    return false;
}
