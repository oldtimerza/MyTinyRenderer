#ifndef __RAW_H__
#define __RAW_H__

#include "image.h"
#include "color.h"

class Raw : public Image
{
protected:
    unsigned char *data;
    int width;
    int height;
    int channels;

public:
    Raw();
    Raw(int width, int height, int channels);
    ~Raw();
    bool set(int x, int y, TGAColor c);
};

#endif