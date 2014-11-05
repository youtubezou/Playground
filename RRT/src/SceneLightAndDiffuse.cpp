#include <cstdio>
#include "Sphere.h"
#include "TextureColor.h"
#include "SceneLightAndDiffuse.h"

SceneLightAndDiffuse::SceneLightAndDiffuse()
{
}

SceneLightAndDiffuse::~SceneLightAndDiffuse()
{
}

void SceneLightAndDiffuse::initSceneName()
{
    _name = new char[128];
    sprintf(_name, "%s", "light_and_diffuse");
}

void SceneLightAndDiffuse::initBackground()
{
    _background = Color(0.0, 0.0, 0.0);
}

void SceneLightAndDiffuse::initShapes()
{
    _textureNum = 3;
    _textures = new Texture*[3];
    _textures[0] = new TextureColor(Color(0.0, 0.0, 0.0));
    _textures[1] = new TextureColor(Color(0.75, 0.2, 0.2));
    _textures[2] = new TextureColor(Color(0.2, 0.2, 0.75));

    _shapeNum = 4;
    _shapes = new Shape*[4];
    _shapes[0] = new Sphere(Vector3(110.0, 130.0, 10.0), 89.9);
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(12.0, 12.0, 12.0));

    _shapes[1] = new Sphere(Vector3(-110.0, 270.0, -40.0), 209.9);
    _shapes[1]->setTexture(_textures[0]);
    _shapes[1]->setMaterial(Material::DIFFUSE);
    _shapes[1]->setEmission(Color(12.0, 12.0, 12.0));

    _shapes[2] = new Sphere(Vector3(0.0, 0.0, -100.0), 39.9);
    _shapes[2]->setTexture(_textures[1]);
    _shapes[2]->setMaterial(Material::DIFFUSE);

    _shapes[3] = new Sphere(Vector3(0.0, -1e5f - 40.0, -100.0), 1e5f);
    _shapes[3]->setTexture(_textures[2]);
    _shapes[3]->setMaterial(Material::DIFFUSE);
}

void SceneLightAndDiffuse::initCamera()
{
    _camera = new Camera(Vector3(0.0, 0.0, 0.0),
                         Vector3(0.0, 0.0, -1.0),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 100.0,
                         -50.0, -50.0, 50.0, 50.0);
}

void SceneLightAndDiffuse::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneLightAndDiffuse::initImage()
{
    _image = new Image(500, 500);
}
