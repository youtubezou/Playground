#ifndef SCENE_H
#define SCENE_H

#ifdef SYSTEM_WIN32
#include <windows.h>
#endif // SYSTEM_WIN32

#include <ctime>
#include "Shape.h"
#include "Camera.h"
#include "Sample.h"
#include "Image.h"

class Scene
{
public:
    Scene();
    virtual ~Scene();

    void init();
    void render(int maxRayNum, int threadNum = 1);

protected:
    virtual void initSceneName();
    virtual void initBackground();
    virtual void initShapes();
    virtual void initCamera();
    virtual void initSample();
    virtual void initImage();

    char* _name;
    Color _background;
    int _shapeNum;
    Shape** _shapes;
    int _textureNum;
    Texture** _textures;
    Camera* _camera;
    Sample* _sample;
    Image* _image;
    int* _randomRow;
    int* _randomCol;

    clock_t _beginTime;

    #ifdef SYSTEM_WIN32
    int _rayNum;
    bool* _occupy;
    int _deltPixel;
    HANDLE _deltMutex;

    static DWORD WINAPI renderThreadEntry(LPVOID self);
    void renderThread();
    #endif // SYSTEM_WIN32

private:
    void initRandomRow();
    void initRandomColumn();
    void printRemainingTime();
};

#endif // SCENE_H
