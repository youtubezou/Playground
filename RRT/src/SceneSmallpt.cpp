#include <cstdio>
#include "Sphere.h"
#include "TextureColor.h"
#include "Filter.h"
#include "SceneSmallpt.h"

SceneSmallpt::SceneSmallpt()
{
}

SceneSmallpt::~SceneSmallpt()
{
}

void SceneSmallpt::initSceneName()
{
    _name = new char[8];
    sprintf(_name, "%s", "smallpt");
}

void SceneSmallpt::initBackground()
{
    _background = Color(0.0f, 0.0f, 0.0f);
}

void SceneSmallpt::initShapes()
{
    _textureNum = 5;
    _textures = new Texture*[5];
    _textures[0] = new TextureColor(Color(0.75f, 0.25f, 0.25f));
    _textures[1] = new TextureColor(Color(0.25f, 0.25f, 0.75f));
    _textures[2] = new TextureColor(Color(0.75f, 0.75f, 0.75f));
    _textures[3] = new TextureColor(Color(0.0f, 0.0f, 0.0f));
    _textures[4] = new TextureColor(Color(0.999f, 0.999f, 0.999f));

    _shapeNum = 9;
    _shapes = new Shape*[9];
    _shapes[0] = new Sphere(Vector3(1e5f + 1.0f, 40.8f, 81.6f), 1e5f);
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setMaterial(Material::DIFFUSE);

    _shapes[1] = new Sphere(Vector3(-1e5f + 99.0f, 40.8f, 81.6f), 1e5f);
    _shapes[1]->setTexture(_textures[1]);
    _shapes[1]->setMaterial(Material::DIFFUSE);

    _shapes[2] = new Sphere(Vector3(50.0f, 40.8f, 1e5f), 1e5f);
    _shapes[2]->setTexture(_textures[2]);
    _shapes[2]->setMaterial(Material::DIFFUSE);

    _shapes[3] = new Sphere(Vector3(50.0f, 40.8f, -1e5f + 170.0f), 1e5f);
    _shapes[3]->setTexture(_textures[3]);
    _shapes[3]->setMaterial(Material::DIFFUSE);

    _shapes[4] = new Sphere(Vector3(50.0f, 1e5f, 81.6f), 1e5f);
    _shapes[4]->setTexture(_textures[2]);
    _shapes[4]->setMaterial(Material::DIFFUSE);

    _shapes[5] = new Sphere(Vector3(50.0f, -1e5f + 81.6f, 81.6f), 1e5f);
    _shapes[5]->setTexture(_textures[2]);
    _shapes[5]->setMaterial(Material::DIFFUSE);

    _shapes[6] = new Sphere(Vector3(27.0f, 16.5f, 47.0f), 16.5f);
    _shapes[6]->setTexture(_textures[4]);
    _shapes[6]->setMaterial(Material::SPECULAR);

    _shapes[7] = new Sphere(Vector3(73.0f, 16.5f, 78.0f), 16.5f);
    _shapes[7]->setTexture(_textures[4]);
    _shapes[7]->setMaterial(Material::REFRACTION);

    _shapes[8] = new Sphere(Vector3(50.0f, 681.6f - 0.27f, 81.6f), 1e5f);
    _shapes[8]->setTexture(_textures[3]);
    _shapes[8]->setMaterial(Material::DIFFUSE);
    _shapes[8]->setEmission(Color(12.0f, 12.0f, 12.0f));
}

void SceneSmallpt::initCamera()
{
    _camera = new Camera(Vector3(50.0f, 52.0f, 295.6f),
                         Vector3(0.0f, -0.042612f, -1.0f).norm(),
                         Vector3(0.0f, 1.0f, 0.0f),
                         1.0f, 210.0f,
                         1.0f, 0.0f, 99.0f, 81.6f);
}

void SceneSmallpt::initSample()
{
    _sample = new Sample(4, Sample::JITTERED);
    Filter filter;
    filter.filter(*_sample, Filter::TENT);
}

void SceneSmallpt::initImage()
{
    _image = new Image(1024, 768);
}
