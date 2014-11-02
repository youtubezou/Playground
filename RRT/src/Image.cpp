#include <cmath>
#include <cstdlib>
#include <fstream>
#include "Image.h"
using namespace std;

Image::Image() :
    _raster(nullptr), _width(0), _height(0)
{
}

Image::~Image()
{
    clear();
}

Image::Image(int width, int height) :
    _raster(nullptr), _width(width), _height(height)
{
    _raster = new Rgb*[width];
    for (int x = 0; x < width; ++x)
    {
        _raster[x] = new Rgb[height];
    }
}

Image::Image(int width, int height, const Rgb& background) :
    _raster(nullptr), _width(width), _height(height)
{
    _raster = new Rgb*[width];
    for (int x = 0; x < width; ++x)
    {
        _raster[x] = new Rgb[height];
        for (int y = 0; y < height; ++y)
        {
            _raster[x][y] = background;
        }
    }
}

void Image::clear()
{
    if (nullptr != _raster)
    {
        for (int x = 0; x < _width; ++x)
        {
            delete[] _raster[x];
        }
        delete[] _raster;
        _raster = nullptr;
        _width = 0;
        _height = 0;
    }
}

int Image::width() const
{
    return _width;
}

int Image::height() const
{
    return _height;
}

Rgb Image::getPixel(int x, int y)
{
    return _raster[x][y];
}

bool Image::set(int x, int y, const Rgb& color)
{
    if (0 <= x && x < _width)
    {
        if (0 <= y && y < _height)
        {
            _raster[x][y] = color;
            return true;
        }
    }
    return false;
}

void Image::gammaCorrect(float gamma)
{
    float power = 1.0 / gamma;
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            _raster[x][y] = Rgb(pow(_raster[x][y].r(), power),
                                pow(_raster[x][y].g(), power),
                                pow(_raster[x][y].b(), power));
        }
    }
}

void Image::writePPM(ostream& out)
{
    out << "P6" << endl;
    out << _width << ' ' << _height << endl;
    out << "255" << endl;
    for (int y = _height - 1; y >= 0; --y)
    {
        for (int x = 0; x < _width; ++x)
        {
            out.put((unsigned char)(255 * _raster[x][y].r()));
            out.put((unsigned char)(255 * _raster[x][y].g()));
            out.put((unsigned char)(255 * _raster[x][y].b()));
        }
    }
}

void Image::readPPM(string fileName)
{
    ifstream in;
    in.open(fileName.c_str(), ios::in | ios::binary);
    if (!in.is_open())
    {
        cerr << "ERROR: Could not open file '" << fileName << "'." << endl;
        exit(-1);
    }
    int num;
    char ch, type;
    char r, g, b;
    in.get(ch);
    in.get(type);
    in >> _width >> _height >> num;
    clear();
    _raster = new Rgb*[_width];
    for (int i = 0; i < _width; ++i)
    {
        _raster[i] = new Rgb[_height];
    }
    in.get(ch);
    for (int y = _height - 1; y >= 0; --y)
    {
        for (int x = 0; x < _width; ++x)
        {
            in.get(r);
            in.get(g);
            in.get(b);
            _raster[x][y] = Rgb((float)((unsigned char)r) / 255.0,
                                (float)((unsigned char)g) / 255.0,
                                (float)((unsigned char)b) / 255.0);
        }
    }
}
