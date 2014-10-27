#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SceneTest.h"
#include "SceneSmallpt.h"
#include "SceneCornellBox.h"
#include "SceneEarth.h"

int main() {
    srand(time(0));
    SceneTest scene;
    //SceneSmallpt scene;
    //SceneCornellBox scene;
    //SceneEarth scene;
    scene.init();
    //scene.render(10, 4);
    scene.render(30000, 16);
    return 0;
}
