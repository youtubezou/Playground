#include <cstdio>
#include "Sphere.h"
#include "TextureColor.h"
#include "SceneSmallpt.h"

SceneSmallpt::SceneSmallpt() {
}

SceneSmallpt::~SceneSmallpt() {
}

void SceneSmallpt::initSceneName() {
    _name = new char[8];
    sprintf(_name, "%s", "smallpt");
}

void SceneSmallpt::initBackground() {
    _background = Color(0.0, 0.0, 0.0);
}

void SceneSmallpt::initShapes() {
    _textureNum = 5;
    _textures = new Texture*[5];
    _textures[0] = new TextureColor(Color(0.75, 0.25, 0.25));
    _textures[1] = new TextureColor(Color(0.25, 0.25, 0.75));
    _textures[2] = new TextureColor(Color(0.75, 0.75, 0.75));
    _textures[3] = new TextureColor(Color(0.999, 0.999, 0.999));
    _textures[4] = new TextureColor(Color(0.0, 0.0, 0.0));

    _shapeNum = 9;
    _shapes = new Shape*[9];
    _shapes[0] = new Sphere(Vector3(50.0, 681.6 - 0.27, 81.6), 600.0);
    _shapes[0]->setTexture(_textures[4]);
    _shapes[0]->setMaterial(Material::DIFFUSE);
    _shapes[0]->setEmission(Color(12.0, 12.0, 12.0));

    _shapes[1] = new Sphere(Vector3(1e5 + 1.0, 40.8, 81.6), 1e5);
    _shapes[1]->setTexture(_textures[0]);
    _shapes[1]->setMaterial(Material::DIFFUSE);

    _shapes[2] = new Sphere(Vector3(-1e5 + 99.0, 40.8, 81.6f), 1e5);
    _shapes[2]->setTexture(_textures[1]);
    _shapes[2]->setMaterial(Material::DIFFUSE);

    _shapes[3] = new Sphere(Vector3(50.0, 40.8, 1e5), 1e5);
    _shapes[3]->setTexture(_textures[2]);
    _shapes[3]->setMaterial(Material::DIFFUSE);

    _shapes[4] = new Sphere(Vector3(50.0, 40.8, -1e5 + 300), 1e5);
    _shapes[4]->setTexture(_textures[4]);
    _shapes[4]->setMaterial(Material::DIFFUSE);

    _shapes[5] = new Sphere(Vector3(50.0, 1e5, 81.6), 1e5);
    _shapes[5]->setTexture(_textures[2]);
    _shapes[5]->setMaterial(Material::DIFFUSE);

    _shapes[6] = new Sphere(Vector3(50.0, -1e5 + 81.6, 81.6), 1e5);
    _shapes[6]->setTexture(_textures[2]);
    _shapes[6]->setMaterial(Material::DIFFUSE);

    _shapes[7] = new Sphere(Vector3(27.0, 16.5, 47.0), 16.5);
    _shapes[7]->setTexture(_textures[3]);
    _shapes[7]->setMaterial(Material::SPECULAR);

    _shapes[8] = new Sphere(Vector3(73.0, 16.5, 78.0), 16.5);
    _shapes[8]->setTexture(_textures[3]);
    _shapes[8]->setMaterial(Material::REFRACTION);
    _shapes[8]->setDielectric(1.5);
}

void SceneSmallpt::initCamera() {
    _camera = new Camera(Vector3(50.0, 52.0, 295.6f),
                         Vector3(0.0, -0.042612, -1.0).norm(),
                         Vector3(0.0, 1.0, 0.0),
                         1.0, 140.0,
                         -66.0, -50.0, 66.0, 50.0);
}

void SceneSmallpt::initSample() {
    _sample = new Sample(4, Sample::SampleType::JITTERED, Sample::FilterType::TENT);
}

void SceneSmallpt::initImage() {
    _image = new Image(1024, 768);
}
