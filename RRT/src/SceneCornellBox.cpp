#include <cstdio>
#include "Parallelogram.h"
#include "TextureColor.h"
#include "SceneCornellBox.h"

SceneCornellBox::SceneCornellBox() {
}

SceneCornellBox::~SceneCornellBox() {
}

void SceneCornellBox::initSceneName() {
    _name = new char[128];
    sprintf(_name, "%s", "cornell_box");
}

void SceneCornellBox::initBackground() {
    _background = Color(0.0, 0.0, 0.0);
}

void SceneCornellBox::initShapes() {
    int num = 16;

    _textureNum = num;
    _textures = new Texture*[num];
    _shapeNum = num;
    _shapes = new Shape*[num];

    _textures[0] = new TextureColor(Color(0.78, 0.78, 0.78));
    _shapes[0] = new Parallelogram(Vector3(343.0, 548.8, 227.0),
                                   Vector3(343.0, 548.8, 332.0),
                                   Vector3(213.0, 548.8, 227.0));
    _shapes[0]->setTexture(_textures[0]);
    _shapes[0]->setEmission(Color(232, 184, 109) / 255 * 18.4);

    _textures[1] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[1] = new Parallelogram(Vector3(552.8, 0.0, 0.0),
                                   Vector3(552.8, 0.0, 559.2),
                                   Vector3(0.0, 0.0, 0.0));
    _shapes[1]->setTexture(_textures[1]);

    _textures[2] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[2] = new Parallelogram(Vector3(556.0, 548.8000, 0.0),
                                   Vector3(556.0, 548.8000, 559.2),
                                   Vector3(0.0, 548.8000, 0.0));
    _shapes[2]->setTexture(_textures[2]);

    _textures[3] = new TextureColor(Color(0.999, 0.999, 0.999));
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

    _textures[6] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[6] = new Parallelogram(Vector3(130, 165, 65),
                                   Vector3(82, 165, 225),
                                   Vector3(290, 165, 114));
    _shapes[6]->setTexture(_textures[6]);

    _textures[7] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[7] = new Parallelogram(Vector3(290, 0, 114),
                                   Vector3(290, 165, 114),
                                   Vector3(240, 0, 272));
    _shapes[7]->setTexture(_textures[7]);

    _textures[8] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[8] = new Parallelogram(Vector3(130, 0, 65),
                                   Vector3(130, 165, 65),
                                   Vector3(290, 0, 114));
    _shapes[8]->setTexture(_textures[8]);

    _textures[9] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[9] = new Parallelogram(Vector3(82, 0, 225),
                                   Vector3(82, 165, 225),
                                   Vector3(130, 0, 65));
    _shapes[9]->setTexture(_textures[9]);

    _textures[10] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[10] = new Parallelogram(Vector3(240, 0, 272),
                                    Vector3(240, 165, 272),
                                    Vector3(82, 0, 225));
    _shapes[10]->setTexture(_textures[10]);

    _textures[11] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[11] = new Parallelogram(Vector3(423, 330, 247),
                                    Vector3(265, 330, 296),
                                    Vector3(472, 330, 406));
    _shapes[11]->setTexture(_textures[11]);

    _textures[12] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[12] = new Parallelogram(Vector3(423, 0, 247),
                                    Vector3(423, 330, 247),
                                    Vector3(472, 0, 406));
    _shapes[12]->setTexture(_textures[12]);

    _textures[13] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[13] = new Parallelogram(Vector3(472, 0, 406),
                                    Vector3(472, 330, 406),
                                    Vector3(314, 0, 456));
    _shapes[13]->setTexture(_textures[13]);

    _textures[14] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[14] = new Parallelogram(Vector3(314, 0, 456),
                                    Vector3(314, 330, 456),
                                    Vector3(265, 0, 296));
    _shapes[14]->setTexture(_textures[14]);

    _textures[15] = new TextureColor(Color(0.999, 0.999, 0.999));
    _shapes[15] = new Parallelogram(Vector3(265, 0, 296),
                                    Vector3(265, 330, 296),
                                    Vector3(423, 0, 247));
    _shapes[15]->setTexture(_textures[15]);
}

void SceneCornellBox::initCamera() {
    _camera = new Camera(Vector3(278, 273, -800),
                         Vector3(0, 0, 1),
                         Vector3(0, 1, 0),
                         0.025, 800.0,
                         -270.0, -270.0, 270.0, 270.0);
}

void SceneCornellBox::initSample() {
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneCornellBox::initImage() {
    _image = new Image(512, 512);
}
