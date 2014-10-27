#ifndef SCENETEST_H
#define SCENETEST_H

#include "Scene.h"

class SceneTest : public Scene {
public:
    SceneTest();
    virtual ~SceneTest();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENETEST_H
