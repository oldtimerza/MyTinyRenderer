#include <stdio.h>
#include "render.h"
#include "geometry.h"
#include "face.h"
#include "color.h"
#include "transform.h"

Render::Render() : width(0), height(0), channels(0){};

Render::Render(int _width, int _height, int _channels) : width(_width), height(_height), channels(_channels){};

Render::~Render(){};

void Render::draw(Face &face, Buffer &buffer)
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