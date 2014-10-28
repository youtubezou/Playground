#include <fstream>
#include <iostream>
#include "Rgb.h"
#include "Image.h"
#include "Vector3.h"
#include "DynamicArray.h"
#include "Shape.h"
#include "Triangle.h"
#include "Sphere.h"
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
    for (int x = 0; x < 500; ++x)
    {
        for (int y = 0; y < 500; ++y)
        {
            float tmax = 100000.0f;
            bool hitted = false;
            Ray ray(Vector3(x, y, 0), dir);
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
                image.set(x, y, record.color);
            }
            else
            {
                image.set(x, y, Rgb(0.2f, 0.2f, 0.2f));
            }
        }
    }
    fstream fout;
    fout.open("result.ppm", fstream::out | fstream::binary);
    image.writePPM(fout);
    fout.close();
    return 0;
}
