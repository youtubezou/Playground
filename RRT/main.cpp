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
#include <cmath>
#include "Sphere.h"
int main()
{
    double PI = acos(-1.0);
    Ray ray(Vector3(10.0 * cos(30.0 / 180.0 * PI) - 1.0, 10.0 * sin(30.0 / 180.0 * PI)), Vector3(1.0));
    Sphere sphere(Vector3(), 10.0);
    sphere.setDielectric(1.5);
    HitRecord record;
    sphere.hitTest(ray, record);
    Vector3 d = ray.d();
    Vector3 n = record.normal;
    double ddn = dot(d, n);

    Vector3 dr = (d - 2.0 * ddn * n).norm();
    Ray r(record.point, dr);

    bool into = dot(ray.d(), n) < 0.0;
    if (!into)
    {
        n = -n;
    }
    ddn = dot(d, n);

    double nc = 1.0;
    double nt = sphere.dielectric();
    double nnt = into ? nc / nt : nt / nc;
    double costheta2 = 1.0 - nnt * nnt * (1.0 - ddn * ddn);
    double costheta = sqrt(costheta2);
    Vector3 td = (nnt * (d - n * ddn) - n * costheta).norm();
    Vector3 test1 = n * ddn;
    Vector3 test2 = n * costheta;
    Vector3 test3 = nnt * (d - n * ddn);
    Vector3 test4 = nnt * (d - n * ddn) - n * costheta;
    return 0;
    srand(time(0));
    //SceneSingleLuminaire scene;
    //SceneLightAndDiffuse scene;
    //SceneDiffuseAndMirror scene;
    //SceneLightAndRefraction scene;
    //SceneGlassBall scene;
    //SceneInside scene;
    SceneSmallpt scene;
    scene.init();
    scene.render(200);
    return 0;
}
