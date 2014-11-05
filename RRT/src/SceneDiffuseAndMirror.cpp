#include <cstdio>
#include "Sphere.h"
#include "TextureColor.h"
#include "SceneDiffuseAndMirror.h"

SceneDiffuseAndMirror::SceneDiffuseAndMirror()
{
}

SceneDiffuseAndMirror::~SceneDiffuseAndMirror()
{
}

void SceneDiffuseAndMirror::initSceneName()
{
    _name = new char[128];
    sprintf(_name, "%s", "diffuse_and_mirror");
}

void SceneDiffuseAndMirror::initBackground()
{
    _background = Color(0.0, 0.0, 0.0);
}

void SceneDiffuseAndMirror::initShapes()
{
    _textureNum = 1;
    _textures = new Texture*[1];
    _textures[0] = new TextureColor(Color(0.8, 0.2, 0.2));

    _shapeNum = 3;
    _shapes = new Shape*[3];
    _shapes[0] = new Sphere(Vector3(0.0, 30.0, -100.0), 29.5);
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(12.0, 12.0, 12.0));

    _shapes[1] = new Sphere(Vector3(-30.0, -30.0, -100.0), 29.5);
    _shapes[1]->setTexture(_textures[0]);
    _shapes[1]->setMaterial(Material::DIFFUSE);

    _shapes[2] = new Sphere(Vector3(30.0, -30.0, -100.0), 29.5);
    _shapes[2]->setTexture(_textures[0]);
    _shapes[2]->setMaterial(Material::SPECULAR);
}

void SceneDiffuseAndMirror::initCamera()
{
    _camera = new Camera(Vector3(0.0, 0.0, 0.0),
                         Vector3(0.0, 0.0, -1.0),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 100.0,
                         -50.0, -50.0, 50.0, 50.0);
}

void SceneDiffuseAndMirror::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneDiffuseAndMirror::initImage()
{
    _image = new Image(500, 500);
}
