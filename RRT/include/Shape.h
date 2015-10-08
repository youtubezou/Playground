#ifndef SHAPE_H
#define SHAPE_H

#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"
#include "Ray.h"

enum class Material {
    DIFFUSE, // Ideal diffuse reflection.
    SPECULAR, // Ideal specular reflection.
    REFRACTION, // Ideal dielectric refraction.
};

class Shape;

struct HitRecord {
    double t;
    Vector3 point;
    Vector3 normal;
    Vector2 uv;
    Texture* texture;
};

class Shape {
public:
    Shape();
    virtual ~Shape();

    Texture* texture() const;
    void setTexture(Texture* texture);

    Material material() const;
    void setMaterial(Material material);

    const Color& emission() const;
    void setEmission(const Color& color);

    double dielectric() const;
    void setDielectric(double dielectric);

    virtual bool hitTest(const Ray& ray, HitRecord& record) const;

private:
    Texture* _texture;
    Material _material;
    Color _emission;
    double _dielectric;
};

#endif // SHAPE_H
