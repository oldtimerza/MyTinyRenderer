#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL2/SDL.h>
#include "face.h"
#include "geometry.h"

class Render
{
    const Vec3f light_dir = Vec3f(0, 0, -1.0f);
    const int delay = 0;
    int width;
    int height;
    int channels;
    SDL_Window *window;
    SDL_Surface *screenSurface;
    void draw_face_to_buffer(Face &face, Buffer &buffer);
    void update_screen(Buffer &buffer);

public:
    Render();
    Render(SDL_Window *window, SDL_Surface *_surface, int _width, int _height, int _channels);
    ~Render();
    void draw(Face &face, Buffer &buffer);
};

#endif