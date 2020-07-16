#include <stdio.h>
#include <SDL2/SDL.h>

#include "./file/tgaimage.h"
#include "./render/color.h"
#include "./render/buffer.h"
#include "./render/model.h"
#include "./render/geometry.h"
#include "./render/face.h"
#include "./render/render.h"

const Color white = Color(255, 255, 255, 255);
const Color red = Color(255, 0, 0, 255);

const int width = 800;
const int height = 800;
const int channels = 3;

bool init_sdl();
void close();

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;

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
        Buffer *buffer = new Buffer(width, height, channels);
        Buffer &buffer_ref = *buffer;
        Render render = Render(gWindow, gScreenSurface, width, height, channels);

        int index = 0;
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
                Face face = model->face(index);

                render.draw(face, buffer_ref);

                index += 1;
                if (index == model->nfaces())
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
                                   SDL_WINDOW_SHOWN);

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

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
};
