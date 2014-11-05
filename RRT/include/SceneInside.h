#ifndef SCENEINSIDE_H
#define SCENEINSIDE_H

#include "Scene.h"

class SceneInside : public Scene
{
public:
    SceneInside();
    virtual ~SceneInside();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENEINSIDE_H
