#include <stdio.h>
#include <SDL2/SDL.h>

#include "./renderer/color.h"
#include "./renderer/tgaimage.h"
#include "./renderer/model.h"
#include "./renderer/geometry.h"
#include "./renderer/transform.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

const int width = 800;
const int height = 800;
const int channels = 3;

Vec3f light_dir(0, 0, -1.0f);

bool init();
void render_face(int index, Model *model, TGAImage &image);
void close();

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;

//this code is horrible to look at and could do with a major refactoring.
int main(int argc, char **argv)
{
    if (!init())
    {
        printf("Failed to init!");
    }
    else
    {
        Model *model = new Model("obj/african_head.obj");
        int nfaces = model->nfaces();
        int index = 0;

        TGAImage *image_ptr = new TGAImage(width, height, TGAImage::RGB);
        TGAImage &buffer_ref = *image_ptr;

        bool quit = false;
        bool done = false;

        SDL_Event event;
        while (!quit)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
            }

            if (!done) //not done render next face
            {
                printf("Face #%i: Start\n", index);
                render_face(index, model, buffer_ref);
                SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)buffer_ref.buffer(),
                                                                width,
                                                                height,
                                                                channels * 8,     // bits per pixel = 24
                                                                width * channels, // pitch
                                                                0x0000FF,         // red mask
                                                                0x00FF00,         // green mask
                                                                0xFF0000,         // blue mask
                                                                0);               // alpha mask (none)
                index += 1;
                if (index == nfaces)
                {
                    done = true;
                }
                SDL_BlitSurface(surface, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
        buffer_ref.flip_vertically();
        buffer_ref.write_tga_file("output.tga");
        delete model;
        delete image_ptr;
        close();
        return 0;
    }
    return 0;
};

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL Sandbox",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   width,
                                   height,
                                   SDL_WINDOW_SHOWN

        );

        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
};

void render_face(int index, Model *model, TGAImage &image)
{
    std::vector<int> face = model->face(index);
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
};

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
};
