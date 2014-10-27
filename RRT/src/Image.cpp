#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "Image.h"
using namespace std;

Image::Image() :
    _w(0), _h(0), _d(nullptr) {
}

Image::Image(int width, int height) :
    _w(width), _h(height), _d(nullptr) {
    _d = new Color*[_w];
    for (int x = 0; x < _w; ++x) {
        _d[x] = new Color[_h];
    }
}

Image::~Image() {
    clear();
}

void Image::clear() {
    if (_d != nullptr) {
        for (int x = 0; x < w(); ++x) {
            delete[] _d[x];
        }
        delete[] _d;
        _d = nullptr;
    }
}

void Image::readPPM(const char* fileName) {
    ifstream fin;
    fin.open(fileName, ios::in | ios::binary);
    if (!fin.is_open()) {
        cerr << "ERROR: Could not open file: " << fileName << endl;
        exit(-1);
    }
    clear();
    int num;
    char ch, type;
    char r, g, b;
    fin.get(ch);
    fin.get(type);
    fin >> _w >> _h >> num;
    _d = new Color*[w()];
    for (int i = 0; i < w(); ++i) {
        _d[i] = new Color[h()];
    }
    fin.get(ch);
    for (int y = h() - 1; y >= 0; --y) {
        for (int x = 0; x < w(); ++x) {
            fin.get(r);
            fin.get(g);
            fin.get(b);
            set(x, y, Color(((double)(unsigned char)r) / 255.0,
                            ((double)(unsigned char)g) / 255.0,
                            ((double)(unsigned char)b) / 255.0));
        }
    }
    fin.close();
}

void Image::writePPM(const char* fileName) {
    ofstream fout;
    fout.open(fileName, ios::out | ios::binary);
    if (!fout.is_open()) {
        cerr << "ERROR: Could not open file: " << fileName << endl;
        exit(-1);
    }
    fout << "P6" << endl;
    fout << w() << ' ' << h() << endl;
    fout << "255" << endl;
    for (int y = h() - 1; y >= 0; --y) {
        for (int x = 0; x <= w(); ++x) {
            fout.put((unsigned char)(255 * get(x, y).r()));
            fout.put((unsigned char)(255 * get(x, y).g()));
            fout.put((unsigned char)(255 * get(x, y).b()));
        }
    }
    fout.close();
}

void Image::writePPM(const char* fileName, double gamma) {
    ofstream fout;
    fout.open(fileName, ios::out | ios::binary);
    if (!fout.is_open()) {
        cerr << "ERROR: Could not open file: " << fileName << endl;
        exit(-1);
    }
    fout << "P6" << endl;
    fout << w() << ' ' << h() << endl;
    fout << "255" << endl;
    for (int y = h() - 1; y >= 0; --y) {
        for (int x = 0; x < w(); ++x) {
            fout.put((unsigned char)(255 * pow(get(x, y).r(), 1.0 / gamma)));
            fout.put((unsigned char)(255 * pow(get(x, y).g(), 1.0 / gamma)));
            fout.put((unsigned char)(255 * pow(get(x, y).b(), 1.0 / gamma)));
        }
    }
    fout.close();
}
