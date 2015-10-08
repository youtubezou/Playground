#include <cstdio>
#include <cmath>
#include <algorithm>
#include "TextureColor.h"
#include "TextureChess.h"
#include "TextureImage.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Transformable.h"
#include "Parallelogram.h"
#include "SceneTest.h"
using namespace std;

SceneTest::SceneTest() {
}

SceneTest::~SceneTest() {
}

void SceneTest::initSceneName() {
    _name = new char[8];
    sprintf(_name, "%s", "test");
}

void SceneTest::initBackground() {
    _background = Color(0.0, 0.0, 0.0);
}

void SceneTest::initShapes() {
    int num = 25;

    _textureNum = num;
    _textures = new Texture*[num];
    _shapeNum = num;
    _shapes = new Shape*[num];

    _textures[0] = new TextureColor(Color(0.78, 0.78, 0.78));
    _shapes[0] = new Parallelogram(Vector3(343.0, 548.8, 107.0),
                                   Vector3(343.0, 548.8, 212.0),
                                   Vector3(213.0, 548.8, 107.0));
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setEmission(Color(242, 184, 109) / 255 * 20.4);

    _textures[1] = new TextureChess(Color(0.0, 0.0, 0.0),
                                    Color(0.999, 0.999, 0.899),
                                    0.125);
    _shapes[1] = new Parallelogram(Vector3(552.8, 0.0, 0.0),
                                   Vector3(552.8, 0.0, 559.2),
                                   Vector3(0.0, 0.0, 0.0));
    _shapes[1]->setTexture(_textures[1]);

    _textures[2] = new TextureColor(Color(0.999, 0.999, 0.899));
    _shapes[2] = new Parallelogram(Vector3(556.0, 548.8000, 0.0),
                                   Vector3(556.0, 548.8000, 559.2),
                                   Vector3(0.0, 548.8000, 0.0));
    _shapes[2]->setTexture(_textures[2]);

    _textures[3] = new TextureColor(Color(0.999, 0.999, 0.899));
    _shapes[3] = new Parallelogram(Vector3(549.6, 0.0, 559.2),
                                   Vector3(0.0, 0.0, 559.2),
                                   Vector3(549.6, 548.8, 559.2));
    _shapes[3]->setTexture(_textures[3]);

    _textures[4] = new TextureColor(Color(0.0, 0.30, 0.0));
    _shapes[4] = new Parallelogram(Vector3(0.0, 0.0, 559.2),
                                   Vector3(0.0, 0.0, 0.0),
                                   Vector3(0.0, 548.8, 559.2));
    _shapes[4]->setTexture(_textures[4]);

    _textures[5] = new TextureColor(Color(0.69, 0.0, 0.0));
    _shapes[5] = new Parallelogram(Vector3(549.6, 0.0, 0.0),
                                   Vector3(549.6, 548.8, 0.0),
                                   Vector3(549.6, 0.0, 559.2));
    _shapes[5]->setTexture(_textures[5]);

    const double DIAMETER = 98.0;
    const double RADIUS = DIAMETER * 0.5;
    const double CENTER_X = 549.6 * 0.5;
    const double CENTER_Y = 548.8 * 0.5;
    const double CENTER_Z = 559.2 * 0.5;
    int idx = 6;
    for (int h = 1; h <= 5; ++h) {
        int num = 3 - abs(h - 3);
        double y = CENTER_Y - (h - 3) * sqrt(2.0) * 0.5 * DIAMETER;
        double sx = CENTER_X - num * DIAMETER * 0.5;
        double sz = CENTER_Z - num * DIAMETER * 0.5;
        for (int i = 0; i < num; ++i) {
            for (int j = 0; j < num; ++j) {
                _textures[idx] = new TextureColor(Color(0.999 - idx * 0.0015, 0.999 - idx * 0.001, 0.899 + idx * 0.005));
                double x = sx + i * DIAMETER + RADIUS;
                double z = sz + j * DIAMETER + RADIUS;
                _shapes[idx] = new Transformable(new Sphere(Vector3(x, y, z), RADIUS));
                _shapes[idx]->setMaterial(Material::REFRACTION);
                _shapes[idx]->setDielectric(1.5);
                ((Transformable*)_shapes[idx])->rotateByPoint(4.0, 30.0, 2.0, {CENTER_X, CENTER_Y, CENTER_Z});
                _shapes[idx]->setTexture(_textures[idx]);
                ++idx;
            }
        }
    }
}

void SceneTest::initCamera() {
    _camera = new Camera(Vector3(278, 273, -800),
                         Vector3(0, 0, 1),
                         Vector3(0, 1, 0),
                         0.025, 800.0,
                         -270.0, -270.0, 270.0, 270.0);
}

void SceneTest::initSample() {
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneTest::initImage() {
    _image = new Image(512, 512);
}
