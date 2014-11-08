#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SceneCornellBox.h"

int main()
{
    srand(time(0));
    SceneCornellBox scene;
    scene.init();
    scene.render(30000);
    return 0;
}
