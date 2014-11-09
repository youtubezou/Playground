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

#ifdef SYSTEM_WIN32
DWORD WINAPI Scene::renderThreadEntry(LPVOID self)
{
    reinterpret_cast<Scene*>(self)->renderThread();
    return 0;
}

void Scene::renderThread()
{
    Radiance rad(_shapes, _shapeNum, _background);
    Color* color = new Color[_sample->num()];
    while (true)
    {
        int x = -1;
        WaitForSingleObject(_deltMutex, INFINITE);
        for (int i = 0; i < _image->w(); ++i)
        {
            if (!_occupy[i])
            {
                _occupy[i] = true;
                x = i;
                break;
            }
        }
        ReleaseMutex(_deltMutex);
        if (x == -1)
        {
            break;
        }
        for (int y = 0; y < _image->h(); ++y)
        {
            for (int i = 0; i < _sample->num(); ++i)
            {
                color[i] = Color();
            }
            for (int r = 0; r < _rayNum; ++r)
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
                color[i] = color[i] / _rayNum;
                color[i].clamp();
                _image->set(x, y, _image->get(x, y) + color[i] / _sample->num());
            }
            WaitForSingleObject(_deltMutex, INFINITE);
            ++_deltPixel;
            ReleaseMutex(_deltMutex);
        }
    }
    delete[] color;
}
#endif // SYSTEM_WIN32

void Scene::render(int maxRayNum, int threadNum)
{
    #ifdef SYSTEM_WIN32
    _rayNum = maxRayNum / _sample->num();
    char name[128];
    sprintf(name, "%s_%d.ppm", _name, maxRayNum);
    _occupy = new bool[_image->w()];
    for (int x = 0; x < _image->w(); ++x)
    {
        _occupy[x] = false;
    }
    _deltMutex = CreateMutex(NULL, FALSE, NULL);
    for (int i = 0; i < threadNum; ++i)
    {
        CreateThread(0, 0, renderThreadEntry, this, 0, 0);
    }
    int totalPixel = _image->w() * _image->h();
    _deltPixel = 0;
    while (true)
    {
        printf("\rRay: %d\tPercent: %.6f%%", maxRayNum, 100.0 * _deltPixel / totalPixel);
        Sleep(200);
        if (_deltPixel == totalPixel)
        {
            break;
        }
        if (rand() < 10)
        {
            _image->writePPM(name, 2.2);
        }
    }
    delete[] _occupy;
    _image->writePPM(name, 2.2);
    #else // SYSTEM_WIN32
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
    #endif
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
