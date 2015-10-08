#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "Radiance.h"
#include "Scene.h"
using namespace std;

Scene::Scene() {
}

Scene::~Scene() {
    delete[] _name;
    if (_shapes != nullptr) {
        for (int i = 0; i < _shapeNum; ++i) {
            delete _shapes[i];
        }
        delete[] _shapes;
    }
    if (_textures != nullptr) {
        for (int i = 0; i < _textureNum; ++i) {
            delete _textures[i];
        }
        delete[] _textures;
    }
    delete _camera;
    delete _sample;
    delete _image;
    delete _randomRow;
    delete _randomCol;
}

void Scene::init() {
    initSceneName();
    initBackground();
    initShapes();
    initCamera();
    initSample();
    initImage();
}

#ifdef SYSTEM_WIN32
DWORD WINAPI Scene::renderThreadEntry(LPVOID self) {
    reinterpret_cast<Scene*>(self)->renderThread();
    return 0;
}

void Scene::renderThread() {
    Radiance rad(_shapes, _shapeNum, _background);
    Color* color = new Color[_sample->num()];
    while (true) {
        int x = -1;
        WaitForSingleObject(_deltMutex, INFINITE);
        for (int i = 0; i < _image->w(); ++i) {
            if (!_occupy[i]) {
                _occupy[i] = true;
                x = _randomCol[i];
                break;
            }
        }
        ReleaseMutex(_deltMutex);
        if (x == -1) {
            break;
        }
        for (int j = 0; j < _image->h(); ++j) {
            int y = _randomRow[j];
            for (int i = 0; i < _sample->num(); ++i) {
                color[i] = Color();
            }
            for (int r = 0; r < _rayNum; ++r) {
                _sample->resample();
                for (int i = 0; i < _sample->num(); ++i) {
                    double a = (x + _sample->x(i)) / _image->w();
                    double b = (y + _sample->y(i)) / _image->h();
                    Ray ray = _camera->getRay(a, b, 0.0, 0.0);
                    color[i] = color[i] + rad.radiance(ray, 0);
                }
            }
            for (int i = 0; i < _sample->num(); ++i) {
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

void Scene::render(int maxRayNum, int threadNum) {
    _beginTime = clock();
#ifdef SYSTEM_WIN32
    initRandomRow();
    initRandomColumn();
    _rayNum = maxRayNum / _sample->num();
    char name[128];
    sprintf(name, "%s_%d.ppm", _name, maxRayNum);
    _occupy = new bool[_image->w()];
    for (int x = 0; x < _image->w(); ++x) {
        _occupy[x] = false;
    }
    _deltMutex = CreateMutex(NULL, FALSE, NULL);
    for (int i = 0; i < threadNum; ++i) {
        CreateThread(0, 0, renderThreadEntry, this, 0, 0);
    }
    int totalPixel = _image->w() * _image->h();
    _deltPixel = 0;
    while (true) {
        system("cls");
        printf("Ray: %d\n", maxRayNum);
        printf("Progress: %d / %d\n", _deltPixel, totalPixel);
        printf("Percent: %.6f%%\n", 100.0 * _deltPixel / totalPixel);
        printRemainingTime();
        putchar('\n');
        Sleep(1000);
        if (_deltPixel == totalPixel) {
            break;
        }
        if (rand() < 10) {
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
    for (int x = 0; x < _image->w(); ++x) {
        for (int y = 0; y < _image->h(); ++y) {
            for (int i = 0; i < _sample->num(); ++i) {
                color[i] = Color();
            }
            for (int r = 0; r < rayNum; ++r) {
                _sample->resample();
                for (int i = 0; i < _sample->num(); ++i) {
                    double a = (x + _sample->x(i)) / _image->w();
                    double b = (y + _sample->y(i)) / _image->h();
                    Ray ray = _camera->getRay(a, b, 0.0, 0.0);
                    color[i] = color[i] + rad.radiance(ray, 0);
                }
            }
            for (int i = 0; i < _sample->num(); ++i) {
                color[i] = color[i] / rayNum;
                color[i].clamp();
                _image->set(x, y, _image->get(x, y) + color[i] / _sample->num());
            }
        }
        printf("\rRay: %d\tPercent: %.6f%%", maxRayNum, 100.0 * (x + 1) / _image->w());
        if (x % 10 == 9) {
            _image->writePPM(name, 2.2);
        }
    }
    delete[] color;
    _image->writePPM(name, 2.2);
#endif
}

void Scene::initSceneName() {
    _name = new char[1];
    _name[0] = 0;
}

void Scene::initBackground() {
    _background = Color(0.0, 0.0, 0.0);
}

void Scene::initShapes() {
    _shapes = nullptr;
    _shapeNum = 0;
    _textures = nullptr;
    _textureNum = 0;
}

void Scene::initCamera() {
    _camera = new Camera(Vector3(0.0, 0.0, 0.0),
                         Vector3(0.0, 0.0, -1.0),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 100.0,
                         -50.0, -50.0, 50.0, 50.0);
}

void Scene::initSample() {
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void Scene::initImage() {
    _image = new Image(512, 512);
}

void Scene::initRandomRow() {
    delete _randomRow;
    _randomRow = new int[_image->h()];
    printf("%d\n", _image->h());
    for (int i = 0; i < _image->h(); ++i) {
        _randomRow[i] = i;
        int j = rand() % (i + 1);
        swap(_randomRow[i], _randomRow[j]);
    }
}

void Scene::initRandomColumn() {
    delete _randomCol;
    _randomCol = new int[_image->w()];
    for (int i = 0; i < _image->w(); ++i) {
        _randomCol[i] = i;
        int j = rand() % (i + 1);
        swap(_randomCol[i], _randomCol[j]);
    }
}

inline void printWithPlural(int num, const char* unit) {
    printf("%d ", num);
    printf("%s", unit);
    if (num > 1) {
        putchar('s');
    }
    putchar(' ');
}

void Scene::printRemainingTime() {
    int totalPixel = _image->w() * _image->h();
    int passedTime = (double)(clock() - _beginTime) / CLOCKS_PER_SEC;
    int remainTime = (double)passedTime * totalPixel / _deltPixel - passedTime;
    int day = remainTime / 86400;
    int hour = remainTime % 86400 / 3600;
    int minute = remainTime % 3600 / 60;
    int sec = remainTime % 60;
    if (remainTime > 0) {
        printf("Remain: ");
    }
    if (remainTime >= 86400) {
        printWithPlural(day, "day");
    }
    if (remainTime >= 3600) {
        printWithPlural(hour, "hr");
    }
    if (remainTime >= 60) {
        printWithPlural(minute, "min");
    }
    if (remainTime > 0) {
        printWithPlural(sec, "sec");
    }
}
