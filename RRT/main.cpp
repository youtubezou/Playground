#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "SceneSingleLuminaire.h"
#include "SceneLightAndDiffuse.h"
#include "SceneLightAndRefraction.h"
#include "SceneDiffuseAndMirror.h"
#include "SceneGlassBall.h"
#include "SceneInside.h"
#include "SceneSmallpt.h"

int main()
{
    srand(time(0));
    //SceneSingleLuminaire scene;
    //SceneLightAndDiffuse scene;
    //SceneDiffuseAndMirror scene;
    //SceneLightAndRefraction scene;
    //SceneGlassBall scene;
    //SceneInside scene;
    SceneSmallpt scene;
    scene.init();
    scene.render(10000);
    return 0;
}
