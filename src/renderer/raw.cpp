#include <iostream>
#include "color.h"
#include "raw.h"

Raw::Raw(){};

Raw::Raw(int _width, int _height, int _channels) : data(NULL), width(_width), height(_height), channels(_channels)
{
    char *data = new char[width * height * channels];
};

Raw::~Raw()
{
    if (data)
    {
        delete[] data;
    }
};

bool Raw::set(int x, int y, TGAColor c)
{
    data[x + y * width] = c;
};