#ifndef SCENECORNELLBOX_H
#define SCENECORNELLBOX_H

#include "Scene.h"

class SceneCornellBox : public Scene {
public:
    SceneCornellBox();
    virtual ~SceneCornellBox();

private:
    virtual void initSceneName() override final;
    virtual void initBackground() override final;
    virtual void initShapes() override final;
    virtual void initCamera() override final;
    virtual void initSample() override final;
    virtual void initImage() override final;
};

#endif // SCENECORNELLBOX_H
