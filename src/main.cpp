#include <stdio.h>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "transform.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

const int width = 800;
const int height = 800;

Vec3f light_dir(0, 0, -1.0f);

int main(int argc, char **argv)
{
    Model *model = new Model("obj/african_head.obj");

    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++)
    {
        //vertices are stored in the form v x y z w
        //faces are store in the form f vertex_index/texture_coord_index/normal_index
        //for each vertex that makes up the face, 3 groups of such information per face
        //we loop through each face and render lines going from each vertex to each other vertex
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            world_coords[j] = model->vert(face[j]);
            screen_coords[j] = world_space_to_screen_space(world_coords[j], width, height);
        }
        Vec3f normal = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        normal.normalize();
        float intensity = (normal * light_dir) * 255;
        if (intensity > 0)
        {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity, intensity, intensity, 255));
        }
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");

    delete model;
    return 0;
}
