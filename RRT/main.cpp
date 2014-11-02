#include <fstream>
#include <iostream>
#include "Rgb.h"
#include "Image.h"
#include "Vector3.h"
#include "DynamicArray.h"
#include "Shape.h"
#include "UVSphere.h"
#include "DynamicSphere.h"
#include "Camera.h"
#include "Sample.h"
#include "Random.h"
#include "ImageTexture.h"
using namespace std;

int main()
{
    Vector3 dir(0.0f, 0.0f, -1.0f);
    DynamicArray<Shape*> shapes;
    ImageTexture* texture = new ImageTexture("earth.ppm");
    shapes.append(new UVSphere(Vector3(0.0f, 0.0f, 0.0f),
                               400.0f,
                               texture));
    Camera camera(Vector3(1200.0f, -1000.0f, 1300.0f),
                  Vector3(0.0f, 0.0f, 0.0f) - Vector3(1200.0f, -1000.0f, 1300.0f),
                  Vector3(0.0f, 0.0f, 1.0f),
                  0.0,
                  -250.0f, -250.0f, 250.0f, 250.0f,
                  1000.0f);

    Image image(500, 500);
    HitRecord record;
    Vector2* samples = new Vector2[36];
    multiJitter(samples, 36);
    cubicSplineFilter(samples, 36);
    Random random;
    for (int x = 0; x <= 500; ++x)
    {
        cout << x << endl;
        for (int y = 0; y <= 500; ++y)
        {
            Rgb averageColor(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < 36; ++s)
            {
                float tmax = 100000.0f;
                bool hitted = false;
                Ray ray = camera.getRay((x + samples[s].x()) / 500.0f, (y + samples[s].y()) / 500.0f, random(), random());
                for (int k = 0; k < shapes.length(); ++k)
                {
                    if (shapes[k]->hit(ray, 0.00001f, tmax, 0.0f, record))
                    {
                        tmax = record.t;
                        hitted = true;
                    }
                }
                if (hitted)
                {
                    averageColor += record.texture->value(record.uv, record.point);
                }
                else
                {
                    averageColor += Rgb(0.1f, 0.1f, 0.1f);
                }
            }
            image.set(x, y, averageColor / 36.0f);
        }
    }
    fstream fout;
    fout.open("result.ppm", fstream::out | fstream::binary);
    image.writePPM(fout);
    fout.close();
    return 0;
}
