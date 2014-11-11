#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SceneTest.h"
#include "SceneSmallpt.h"
#include "SceneCornellBox.h"

int main()
{
    srand(time(0));
    SceneTest scene;
    //SceneSmallpt scene;
    //SceneCornellBox scene;
    scene.init();
    scene.render(30000, 4);
    return 0;
}
