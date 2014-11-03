#ifndef SCENESINGLELUMINAIRE_H
#define SCENESINGLELUMINAIRE_H

#include "Scene.h"

class SceneSingleLuminaire : public Scene
{
public:
    SceneSingleLuminaire();
    virtual ~SceneSingleLuminaire();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENESINGLELUMINAIRE_H
