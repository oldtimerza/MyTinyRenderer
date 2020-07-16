#include <SDL2/SDL.h>
#include <stdio.h>
#include "render.h"
#include "geometry.h"
#include "face.h"
#include "color.h"
#include "transform.h"

Render::Render() : window(nullptr), screenSurface(nullptr), width(0), height(0), channels(0){};

Render::Render(SDL_Window *_window, SDL_Surface *_surface, int _width, int _height, int _channels) : window(_window), screenSurface(_surface), width(_width), height(_height), channels(_channels){};

Render::~Render(){};

void Render::draw_face_to_buffer(Face &face, Buffer &buffer)
{
    Vec2i screen_coords[3];
    for (int j = 0; j < 3; j++)
    {
        screen_coords[j] = world_space_to_screen_space(face.vertices[j], width, height);
    }
    Vec3f normal = (face.vertices[2] - face.vertices[0]) ^ (face.vertices[1] - face.vertices[0]);
    normal.normalize();
    float intensity = (normal * light_dir) * 255;
    if (intensity > 0)
    {
        triangle(screen_coords[0], screen_coords[1], screen_coords[2], buffer, Color(intensity, intensity, intensity, 255));
    }
};

void Render::update_screen(Buffer &buffer)
{
    //this is probably a bad way to do the render
    //but it works for now
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)buffer.get(),
                                                    width,
                                                    height,
                                                    channels * 8,     // bits per pixel = 24
                                                    width * channels, // pitch
                                                    0x0000FF,         // red mask
                                                    0x00FF00,         // green mask
                                                    0xFF0000,         // blue mask
                                                    0);               // alpha mask (none)
    //do an image flip just for the output then flip back to continue rendering
    //we should change the renderer in future to just render the correct way around
    buffer.flip_vertically();
    SDL_BlitSurface(surface, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);
    buffer.flip_vertically();
};

void Render::draw(Face &face, Buffer &buffer)
{
    draw_face_to_buffer(face, buffer);

    SDL_Delay(delay);

    update_screen(buffer);
};