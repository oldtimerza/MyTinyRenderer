#include <stdio.h>
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

int main(int argc, char **argv)
{
    TGAImage image(100, 100, TGAImage::RGB);
    line(20, 0, 50, 50, image, red);
    line(50, 50, 20, 0, image, white);
    image.write_tga_file("output.tga");
    return 0;
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    //Transpose the line when working with "steep" lines to make then shallow
    //then flip the x,y in the image set if steep=true
    //otherwise gaps in the lines appear if the angle is too steep
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    //Swap points to always draw from left to right
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    //We multiply by 2 here so we can avoit doing a compare against 0.5
    //to determine if the next line segment is closer to the horizontal
    //or vertical pixel
    int derror = std::abs(dy) * 2;
    int error = 0;
    int y = y0;
    for (float x = x0; x <= x1; x++)
    {
        //if steep we need to de-transpose
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error += derror;
        if (error > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            error -= dx * 2;
        }
    }
}