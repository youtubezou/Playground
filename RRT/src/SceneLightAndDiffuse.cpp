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
    _background = Color(0.0f, 0.0f, 0.0f);
}

void SceneLightAndDiffuse::initShapes()
{
    _textureNum = 5;
    _textures = new Texture*[1];
    _textures[0] = new TextureColor(Color(1.0, 1.0, 1.0));

    _shapeNum = 2;
    _shapes = new Shape*[2];
    _shapes[0] = new Sphere(Vector3(0.0f, 30.0f, -100.0f), 29.5f);
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(12.0f, 12.0f, 12.0f));

    _shapes[1] = new Sphere(Vector3(0.0f, -30.0f, -100.0f), 29.5f);
    _shapes[1]->setTexture(_textures[0]);
    _shapes[1]->setMaterial(Material::DIFFUSE);
}

void SceneLightAndDiffuse::initCamera()
{
    _camera = new Camera(Vector3(0.0f, 0.0f, 0.0f),
                         Vector3(0.0f, 0.0f, -1.0f),
                         Vector3(0.0f, 1.0f, 0.0f),
                         1.0f, 100.0f,
                         -50.0f, -50.0f, 50.0f, 50.0f);
}

void SceneLightAndDiffuse::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneLightAndDiffuse::initImage()
{
    _image = new Image(500, 500);
}
