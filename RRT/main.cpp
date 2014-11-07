#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SceneSmallpt.h"

int main()
{
    srand(time(0));
    SceneSmallpt scene;
    scene.init();
    scene.render(20);
    return 0;
}
