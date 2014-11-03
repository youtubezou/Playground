#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SceneSingleLuminaire.h"
#include "SceneLightAndDiffuse.h"
#include "SceneSmallpt.h"

int main()
{
    srand(time(0));
    // SceneSingleLuminaire scene;
    SceneLightAndDiffuse scene;
    // SceneSmallpt scene;
    scene.init();
    scene.render();
    return 0;
}
