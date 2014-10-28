#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "Rgb.h"

class Image
{
public:
    Image();
    virtual ~Image();
    Image(int width, int height);
    Image(int width, int height, const Rgb& background);

    void clear();
    bool set(int x, int y, const Rgb& color);
    void gammaCorrect(float gamma);

    void writePPM(std::ostream& out);
    void readPPM(std::string fileName);

private:
    Rgb** _raster;
    int _width;
    int _height;
};

#endif // IMAGE_H
