#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>

#include "../render/color.h"
#include "../render/buffer.h"

#pragma pack(push, 1)
struct TGA_Header
{
    char idlength;
    char colormaptype;
    char datatypecode;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char bitsperpixel;
    char imagedescriptor;
};
#pragma pack(pop)

class TGAImage
{
protected:
    Buffer *buffer;
    bool unload_rle_data(std::ofstream &out);

public:
    enum Format
    {
        GRAYSCALE = 1,
        RGB = 3,
        RGBA = 4
    };
    TGAImage();
    TGAImage(Buffer *_buffer);
    bool write_tga_file(const char *filename, bool rle = true);
    ~TGAImage();
    Buffer *get_buffer();
};

#endif //__IMAGE_H__