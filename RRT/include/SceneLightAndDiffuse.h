#ifndef SCENELIGHTANDDIFFUSE_H
#define SCENELIGHTANDDIFFUSE_H

#include "Scene.h"

class SceneLightAndDiffuse : public Scene
{
public:
    SceneLightAndDiffuse();
    virtual ~SceneLightAndDiffuse();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENELIGHTANDDIFFUSE_H
