#ifndef SCENEEARTH_H
#define SCENEEARTH_H

#include "Scene.h"

class SceneEarth : public Scene {
public:
    SceneEarth();
    virtual ~SceneEarth();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENEEARTH_H
