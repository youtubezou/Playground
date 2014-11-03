#ifndef SCENE_H
#define SCENE_H

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
    void render(int maxRayNum = 1e9);

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
};

#endif // SCENE_H
