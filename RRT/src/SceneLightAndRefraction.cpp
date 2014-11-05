#include <cstdio>
#include "Sphere.h"
#include "TextureColor.h"
#include "SceneLightAndRefraction.h"

SceneLightAndRefraction::SceneLightAndRefraction()
{
}

SceneLightAndRefraction::~SceneLightAndRefraction()
{
}

void SceneLightAndRefraction::initSceneName()
{
    _name = new char[128];
    sprintf(_name, "%s", "light_and_refraction");
}

void SceneLightAndRefraction::initBackground()
{
    _background = Color(0.0, 0.0, 0.0);
}

void SceneLightAndRefraction::initShapes()
{
    _textureNum = 4;
    _textures = new Texture*[4];
    _textures[0] = new TextureColor(Color(0.0, 0.0, 0.0));
    _textures[1] = new TextureColor(Color(0.999, 0.999, 0.999));
    _textures[2] = new TextureColor(Color(0.8, 0.8, 0.8));
    _textures[3] = new TextureColor(Color(0.2, 0.2, 0.8));

    _shapeNum = 4;
    _shapes = new Shape*[4];
    _shapes[0] = new Sphere(Vector3(100.0, 90.0, 0.0), 30.0);
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(12.0, 12.0, 12.0));

    _shapes[1] = new Sphere(Vector3(0.0, 0.0, -100.0), 30.0);
    _shapes[1]->setTexture(_textures[1]);
    _shapes[1]->setMaterial(Material::REFRACTION);
    _shapes[1]->setDielectric(1.5);

    _shapes[2] = new Sphere(Vector3(-20.0, 0.0, -130.0), 30.0);
    _shapes[2]->setTexture(_textures[3]);
    _shapes[2]->setMaterial(Material::DIFFUSE);

    _shapes[3] = new Sphere(Vector3(0.0, -1e5f - 40.0, -100.0), 1e5f);
    _shapes[3]->setTexture(_textures[2]);
    _shapes[3]->setMaterial(Material::DIFFUSE);
}

void SceneLightAndRefraction::initCamera()
{
    _camera = new Camera(Vector3(0.0, 0.0, 0.0),
                         Vector3(0.0, 0.0, -1.0),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 100.0,
                         -50.0, -50.0, 50.0, 50.0);
}

void SceneLightAndRefraction::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneLightAndRefraction::initImage()
{
    _image = new Image(500, 500);
}
