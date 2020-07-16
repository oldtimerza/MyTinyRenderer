#ifndef __IMAGE_INTERFACE_H__
#define __IMAGE_INTERFACE_H__

#include "color.h"

class Image
{
public:
    Image(){};
    virtual ~Image(){};
    virtual bool set(int x, int y, Color c);
};

#endif