#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SceneCornellBox.h"

int main()
{
    srand(time(0));
    SceneCornellBox scene;
    scene.init();
    scene.render(100, 6);
    return 0;
}
