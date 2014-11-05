#include <cstdio>
#include "Sphere.h"
#include "TextureColor.h"
#include "SceneInside.h"

SceneInside::SceneInside()
{
}

SceneInside::~SceneInside()
{
}

void SceneInside::initSceneName()
{
    _name = new char[128];
    sprintf(_name, "%s", "inside");
}

void SceneInside::initBackground()
{
    _background = Color(0.0, 0.0, 0.0);
}

void SceneInside::initShapes()
{
    _textureNum = 1;
    _textures = new Texture*[1];
    _textures[0] = new TextureColor(Color(0.0, 0.0, 0.0));

    _shapeNum = 6;
    _shapes = new Shape*[6];
    _shapes[0] = new Sphere(Vector3(1e5f + 1.0, 40.8, 81.6f), 1e5f);
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(0.75f, 0.25f, 0.25f));

    _shapes[1] = new Sphere(Vector3(-1e5f + 99.0, 40.8, 81.6f), 1e5f);
    _shapes[1]->setTexture(_textures[0]);
    _shapes[1]->setMaterial(Material::DIFFUSE);
    _shapes[1]->setEmission(Color(0.25f, 0.25f, 0.75f));

    _shapes[2] = new Sphere(Vector3(50.0, 40.8, 1e5f), 1e5f);
    _shapes[2]->setTexture(_textures[0]);
    _shapes[2]->setMaterial(Material::DIFFUSE);
    _shapes[2]->setEmission(Color(0.75f, 0.75f, 0.75f));

    _shapes[3] = new Sphere(Vector3(50.0, 40.8, -1e5f + 170.0), 1.0);
    _shapes[3]->setTexture(_textures[0]);
    _shapes[3]->setMaterial(Material::DIFFUSE);
    _shapes[3]->setEmission(Color(0.2, 0.8, 0.2));

    _shapes[4] = new Sphere(Vector3(50.0, 1e5f, 81.6f), 1e5f);
    _shapes[4]->setTexture(_textures[0]);
    _shapes[4]->setMaterial(Material::DIFFUSE);
    _shapes[4]->setEmission(Color(0.75f, 0.75f, 0.75f));

    _shapes[5] = new Sphere(Vector3(50.0, -1e5f + 81.6f, 81.6f), 1e5f);
    _shapes[5]->setTexture(_textures[0]);
    _shapes[5]->setMaterial(Material::DIFFUSE);
    _shapes[5]->setEmission(Color(0.75f, 0.75f, 0.75f));
}

void SceneInside::initCamera()
{
    _camera = new Camera(Vector3(50.0, 52.0, 295.6f),
                         Vector3(0.0, -0.042612f, -1.0).norm(),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 140.0,
                         -50.0, -52.0, 50.0, 52.0);
}

void SceneInside::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneInside::initImage()
{
    _image = new Image(500, 500);
}
