#include <cstdio>
#include "Radiance.h"
#include "Scene.h"

Scene::Scene() :
    _name(nullptr), _background(),
    _shapeNum(0), _shapes(nullptr), _textureNum(0), _textures(nullptr),
    _camera(nullptr), _sample(nullptr), _image(nullptr)
{
}

Scene::~Scene()
{
    delete[] _name;
    if (_shapes != nullptr)
    {
        for (int i = 0; i < _shapeNum; ++i)
        {
            delete _shapes[i];
        }
        delete[] _shapes;
    }
    if (_textures != nullptr)
    {
        for (int i = 0; i < _textureNum; ++i)
        {
            delete _textures[i];
        }
        delete[] _textures;
    }
    delete _camera;
    delete _sample;
    delete _image;
}

void Scene::init()
{
    initSceneName();
    initBackground();
    initShapes();
    initCamera();
    initSample();
    initImage();
}

void Scene::render(int maxRayNum)
{
    int rayNum = 0;
    char name[128];
    Radiance rad(_shapes, _shapeNum, _background);
    while (rayNum < maxRayNum)
    {
        for (int x = 0; x < _image->w(); ++x)
        {
            for (int y = 0; y < _image->h(); ++y)
            {
                for (int i = 0; i < _sample->num(); ++i)
                {
                    float a = (x + _sample->x(i)) / _image->w();
                    float b = (y + _sample->y(i)) / _image->h();
                    Ray ray = _camera->getRay(a, b, 0.0f, 0.0f);
                    Color color = rad.radiance(ray, 0);
                    color.clamp();
                    _image->set(x, y, _image->get(x, y) + color);
                }
            }
            printf("\rRay: %d\tPercent: %.2f%%", rayNum, 100.0 * x / _image->w());
        }
        rayNum += _sample->num();
        sprintf(name, "%s_%d.ppm", _name, rayNum);
        _image->writePPM(name, rayNum);
    }
}

void Scene::initSceneName()
{
    _name = new char[1];
    _name[0] = 0;
}

void Scene::initBackground()
{
    _background = Color(0.0f, 0.0f, 0.0f);
}

void Scene::initShapes()
{
    _shapes = nullptr;
    _shapeNum = 0;
    _textures = nullptr;
    _textureNum = 0;
}

void Scene::initCamera()
{
    _camera = nullptr;
}

void Scene::initSample()
{
    _sample = nullptr;
}

void Scene::initImage()
{
    _image = nullptr;
}
