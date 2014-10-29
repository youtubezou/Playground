#include <fstream>
#include <iostream>
#include "Rgb.h"
#include "Image.h"
#include "Vector3.h"
#include "DynamicArray.h"
#include "Shape.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Sample.h"
using namespace std;

int main()
{
    Vector3 dir(0.0f, 0.0f, -1.0f);
    DynamicArray<Shape*> shapes;
    shapes.append(new Sphere(Vector3(250.0f, 250.0f, -1000.0f),
                             150.0f,
                             Rgb(0.2f, 0.2f, 0.8f)));
    shapes.append(new Triangle(Vector3(300.0f, 600.0f, -800.0f),
                               Vector3(0.0f, 100.0f, -1000.0f),
                               Vector3(450.0f, 20.0f, -1000.0f),
                               Rgb(0.8f, 0.2f, 0.2f)));
    Image image(500, 500);
    HitRecord record;
    Vector2* samples = new Vector2[36];
    multiJitter(samples, 36);
    cubicSplineFilter(samples, 36);
    for (int x = 0; x < 500; ++x)
    {
        for (int y = 0; y < 500; ++y)
        {
            Rgb averageColor(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < 36; ++s)
            {
                float tmax = 100000.0f;
                bool hitted = false;
                Ray ray(Vector3(x + samples[s].x(), y + samples[s].y(), 0), dir);
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
                    averageColor += record.color;
                }
                else
                {
                    averageColor += Rgb(0.2f, 0.2f, 0.2f);
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
