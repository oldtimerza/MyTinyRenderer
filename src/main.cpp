#include <stdio.h>
#include <SDL2/SDL.h>

#include "./file/tgaimage.h"
#include "./render/color.h"
#include "./render/buffer.h"
#include "./render/model.h"
#include "./render/geometry.h"
#include "./render/transform.h"

const Color white = Color(255, 255, 255, 255);
const Color red = Color(255, 0, 0, 255);

const int width = 800;
const int height = 800;
const int channels = 3;

Vec3f light_dir(0, 0, -1.0f);

bool init_sdl();
void render_face(int index, Model *model, Buffer &buffer);
void close();

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;

const int delay = 10;

//this code is horrible to look at and could do with a major refactoring.
int main(int argc, char **argv)
{
    if (!init_sdl())
    {
        printf("Failed to init_sdl!");
    }
    else
    {
        Model *model = new Model("obj/african_head.obj");
        int nfaces = model->nfaces();
        int index = 0;

        Buffer *buffer = new Buffer(width, height, channels);
        Buffer &buffer_ref = *buffer;

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
                SDL_Delay(delay);
                printf("Face #%i: Start\n", index);
                render_face(index, model, buffer_ref);

                //this is probably a memory leak and needs to be fixed sometime.
                SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)buffer_ref.get(),
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
                buffer_ref.flip_vertically();
                SDL_BlitSurface(surface, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface(gWindow);
                buffer_ref.flip_vertically();

                index += 1;
                if (index == nfaces)
                {
                    done = true;
                }
            }
        }
        TGAImage image = TGAImage(buffer);
        image.write_tga_file("output.tga");
        delete buffer;
        delete model;
        close();
        return 0;
    }
    return 0;
};

bool init_sdl()
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

void render_face(int index, Model *model, Buffer &buffer)
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
        triangle(screen_coords[0], screen_coords[1], screen_coords[2], buffer, Color(intensity, intensity, intensity, 255));
    }
};

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
};
