#include <cstdio>
#include "Sphere.h"
#include "TextureColor.h"
#include "SceneGlassBall.h"

SceneGlassBall::SceneGlassBall()
{
}

SceneGlassBall::~SceneGlassBall()
{
}

void SceneGlassBall::initSceneName()
{
    _name = new char[128];
    sprintf(_name, "%s", "glass_ball");
}

void SceneGlassBall::initBackground()
{
    _background = Color(0.0, 0.0, 0.0);
}

void SceneGlassBall::initShapes()
{
    _textureNum = 2;
    _textures = new Texture*[2];
    _textures[0] = new TextureColor(Color(0.999, 0.999, 0.999));
    _textures[1] = new TextureColor(Color(0.0, 0.0, 0.0));

    _shapeNum = 3;
    _shapes = new Shape*[3];
    _shapes[0] = new Sphere(Vector3(50.0, 681.6 - 0.27, 81.6), 600.0);
    _shapes[0]->setTexture(_textures[1]);
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(12.0, 12.0, 12.0));

    _shapes[1] = new Sphere(Vector3(27.0, 16.5, 47.0), 16.5);
    _shapes[1]->setTexture(_textures[0]);
    _shapes[1]->setMaterial(Material::SPECULAR);

    _shapes[2] = new Sphere(Vector3(73.0, 16.5, 78.0), 16.5);
    _shapes[2]->setTexture(_textures[0]);
    _shapes[2]->setMaterial(Material::REFRACTION);
    _shapes[2]->setDielectric(1.5);
}

void SceneGlassBall::initCamera()
{
    _camera = new Camera(Vector3(50.0, 52.0, 295.6f),
                         Vector3(0.0, -0.042612f, -1.0).norm(),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 140.0,
                         -50.0, -52.0, 50.0, 52.0);
}

void SceneGlassBall::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneGlassBall::initImage()
{
    _image = new Image(500, 500);
}
