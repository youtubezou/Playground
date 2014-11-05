#include <cstdio>
#include "Sphere.h"
#include "TextureColor.h"
#include "SceneSingleLuminaire.h"

SceneSingleLuminaire::SceneSingleLuminaire()
{
}

SceneSingleLuminaire::~SceneSingleLuminaire()
{
}

void SceneSingleLuminaire::initSceneName()
{
    _name = new char[17];
    sprintf(_name, "%s", "single_luminaire");
}

void SceneSingleLuminaire::initBackground()
{
    _background = Color(0.0, 0.0, 0.0);
}

void SceneSingleLuminaire::initShapes()
{
    _textureNum = 5;
    _textures = new Texture*[1];
    _textures[0] = new TextureColor(Color(0.0, 0.0, 0.0));

    _shapeNum = 1;
    _shapes = new Shape*[1];
    _shapes[0] = new Sphere(Vector3(0.0, 0.0, -100.0), 30.0);
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(12.0, 12.0, 12.0));
}

void SceneSingleLuminaire::initCamera()
{
    _camera = new Camera(Vector3(0.0, 0.0, 0.0),
                         Vector3(0.0, 0.0, -1.0),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 100.0,
                         -50.0, -50.0, 50.0, 50.0);
}

void SceneSingleLuminaire::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneSingleLuminaire::initImage()
{
    _image = new Image(500, 500);
}
