#ifndef __RENDER_H__
#define __RENDER_H__

#include "face.h"
#include "geometry.h"

class Render
{
    const Vec3f light_dir = Vec3f(0, 0, -1.0f);
    const int delay = 0;
    int width;
    int height;
    int channels;

public:
    Render();
    Render(int _width, int _height, int _channels);
    ~Render();
    void draw(Face &face, Buffer &buffer);
};

#endif