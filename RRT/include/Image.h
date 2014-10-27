#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"

class Image {
public:
    Image();
    Image(int width, int height);
    virtual ~Image();

    void clear();

    int w() const;
    int h() const;
    const Color& get(int x, int y) const;
    void set(int x, int y, const Color& color);

    void readPPM(const char* fileName);
    void writePPM(const char* fileName);
    void writePPM(const char* fileName, double gamma);

private:
    int _w, _h;
    Color** _d;
};

inline int Image::w() const {
    return _w;
}

inline int Image::h() const {
    return _h;
}

inline const Color& Image::get(int x, int y) const {
    return _d[x][y];
}

inline void Image::set(int x, int y, const Color& color) {
    _d[x][y] = color;
}

#endif // IMAGE_H
