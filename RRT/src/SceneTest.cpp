#include <cstdio>
#include "TextureColor.h"
#include "Sphere.h"
#include "Triangle.h"
#include "SceneTest.h"

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

void SceneTest::initSceneName()
{
    _name = new char[8];
    sprintf(_name, "%s", "test");
}

void SceneTest::initBackground()
{
    _background = Color(0.0, 0.0, 0.0);
}

void SceneTest::initShapes()
{
    _textureNum = 0;

    _shapeNum = 1;
    _shapes = new Shape*[1];
    _shapes[0] = new Triangle(Vector3(-50, 0, -100.0),
                              Vector3(0, 50, -100.0),
                              Vector3(-30, -50, -100.0));
    _shapes[0]->setTexture(new TextureColor(Color(0.5, 0.5, 0.9)));
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(0.5, 0.5, 0.9));
}

void SceneTest::initCamera()
{
    _camera = new Camera(Vector3(0.0, 0.0, 0.0f),
                         Vector3(0.0, 0.0, -1.0),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 100.0,
                         -250.0, -250.0, 250.0, 250.0);
}

void SceneTest::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneTest::initImage()
{
    _image = new Image(500, 500);
}
