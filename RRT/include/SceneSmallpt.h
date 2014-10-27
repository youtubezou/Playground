#ifndef SCENESMALLPT_H
#define SCENESMALLPT_H

#include "Scene.h"

class SceneSmallpt : public Scene {
public:
    SceneSmallpt();
    virtual ~SceneSmallpt();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENESMALLPT_H
