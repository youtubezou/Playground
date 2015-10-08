#include <cstdio>
#include "TextureColor.h"
#include "TextureChess.h"
#include "TextureImage.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Transformable.h"
#include "Parallelogram.h"
#include "SceneEarth.h"

SceneEarth::SceneEarth() {
}

SceneEarth::~SceneEarth() {
}

void SceneEarth::initSceneName() {
    _name = new char[8];
    sprintf(_name, "%s", "earth");
}

void SceneEarth::initBackground() {
    _background = Color(0.0, 0.0, 0.0);
}

void SceneEarth::initShapes() {
    int num = 7;

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

    _textures[6] = new TextureImage("earth.ppm");
    _shapes[6] = new Transformable(new Sphere(Vector3(0.0, 0.0, 0.0), 150.0));
    ((Transformable*)_shapes[6])->rotateY(180.0 / 180.0 * 3.14);
    ((Transformable*)_shapes[6])->rotateZ(13.0 / 180.0 * 3.14);
    ((Transformable*)_shapes[6])->translate(270, 270, 400);
    _shapes[6]->setTexture(_textures[6]);
}

void SceneEarth::initCamera() {
    _camera = new Camera(Vector3(278, 273, -800),
                         Vector3(0, 0, 1),
                         Vector3(0, 1, 0),
                         0.025, 800.0,
                         -270.0, -270.0, 270.0, 270.0);
}

void SceneEarth::initSample() {
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneEarth::initImage() {
    _image = new Image(512, 512);
}
