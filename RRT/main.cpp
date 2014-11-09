#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SceneSmallpt.h"
#include "SceneCornellBox.h"

int main()
{
    srand(time(0));
    SceneSmallpt scene;
    //SceneCornellBox scene;
    scene.init();
    scene.render(30000, 7);
    return 0;
}
