#ifndef SCENEGLASSBALL_H
#define SCENEGLASSBALL_H

#include "Scene.h"

class SceneGlassBall : public Scene
{
public:
    SceneGlassBall();
    virtual ~SceneGlassBall();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENEGLASSBALL_H
