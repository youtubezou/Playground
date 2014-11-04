#ifndef SCENEDIFFUSEANDMIRROR_H
#define SCENEDIFFUSEANDMIRROR_H

#include "Scene.h"

class SceneDiffuseAndMirror : public Scene
{
public:
    SceneDiffuseAndMirror();
    virtual ~SceneDiffuseAndMirror();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENEDIFFUSEANDMIRROR_H
