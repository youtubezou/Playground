#include <fstream>
#include <iostream>
#include "Rgb.h"
#include "Image.h"
#include "Vector3.h"
#include "DynamicArray.h"
#include "Shape.h"
#include "Triangle.h"
#include "Sphere.h"
#include "DynamicSphere.h"
#include "Camera.h"
#include "Sample.h"
#include "Random.h"
using namespace std;

int main()
{
    Vector3 dir(0.0f, 0.0f, -1.0f);
    DynamicArray<Shape*> shapes;
    shapes.append(new DynamicSphere(Vector3(250.0f, 250.0f, -1000.0f),
                                    150.0f,
                                    Rgb(0.2f, 0.2f, 0.8f),
                                    0.0f, 20.0f));
    shapes.append(new Sphere(Vector3(200.0f, 200.0f, -500.0f),
                             30.0f,
                             Rgb(0.2f, 0.8f, 0.2f)));
    shapes.append(new Triangle(Vector3(300.0f, 600.0f, -1400.0f),
                               Vector3(0.0f, 100.0f, -1800.0f),
                               Vector3(450.0f, 20.0f, -1800.0f),
                               Rgb(0.8f, 0.2f, 0.2f)));
    Camera camera(Vector3(250.0f, 250.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 2.0, -250.0f, -250.0f, 250.0f, 250.0f, 1000.0f);

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
                for (int t = 0; t <= 20; ++t)
                {
                    for (int k = 0; k < shapes.length(); ++k)
                    {
                        if (shapes[k]->hit(ray, 0.00001f, tmax, t / 20.0f, record))
                        {
                            tmax = record.t;
                            hitted = true;
                        }
                    }
                    if (hitted)
                    {
                        averageColor += record.color;
                    }
                    else
                    {
                        averageColor += Rgb(0.2f, 0.2f, 0.2f);
                    }
                }
            }
            image.set(x, y, averageColor / 36.0f / 20.0f);
        }
    }
    fstream fout;
    fout.open("result.ppm", fstream::out | fstream::binary);
    image.writePPM(fout);
    fout.close();
    return 0;
}
