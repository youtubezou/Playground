#ifndef SCENELIGHTANDREFRACTION_H
#define SCENELIGHTANDREFRACTION_H

#include "Scene.h"

class SceneLightAndRefraction : public Scene
{
public:
    SceneLightAndRefraction();
    virtual ~SceneLightAndRefraction();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENELIGHTANDREFRACTION_H
