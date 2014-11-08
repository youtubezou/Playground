#include <cstdio>
#include <cstdlib>
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
    int rayNum = maxRayNum / _sample->num();
    char name[128];
    sprintf(name, "%s_%d.ppm", _name, maxRayNum);
    Radiance rad(_shapes, _shapeNum, _background);
    Color* color = new Color[_sample->num()];
    for (int x = 0; x < _image->w(); ++x)
    {
        for (int y = 0; y < _image->h(); ++y)
        {
            for (int i = 0; i < _sample->num(); ++i)
            {
                color[i] = Color();
            }
            for (int r = 0; r < rayNum; ++r)
            {
                _sample->resample();
                for (int i = 0; i < _sample->num(); ++i)
                {
                    double a = (x + _sample->x(i)) / _image->w();
                    double b = (y + _sample->y(i)) / _image->h();
                    Ray ray = _camera->getRay(a, b, 0.0, 0.0);
                    color[i] = color[i] + rad.radiance(ray, 0);
                }
            }
            for (int i = 0; i < _sample->num(); ++i)
            {
                color[i] = color[i] / rayNum;
                color[i].clamp();
                _image->set(x, y, _image->get(x, y) + color[i] / _sample->num());
            }
        }
        printf("\rRay: %d\tPercent: %.6f%%", maxRayNum, 100.0 * (x + 1) / _image->w());
        if (x % 10 == 9)
        {
            _image->writePPM(name, 2.2);
        }
    }
    delete[] color;
    _image->writePPM(name, 2.2);
}

void Scene::initSceneName()
{
    _name = new char[1];
    _name[0] = 0;
}

void Scene::initBackground()
{
    _background = Color(0.0, 0.0, 0.0);
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
    _camera = new Camera(Vector3(0.0, 0.0, 0.0),
                         Vector3(0.0, 0.0, -1.0),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 100.0,
                         -50.0, -50.0, 50.0, 50.0);
}

void Scene::initSample()
{
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void Scene::initImage()
{
    _image = new Image(500, 500);
}
