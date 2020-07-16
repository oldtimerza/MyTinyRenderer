#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "tgaimage.h"
#include "../render/buffer.h"

TGAImage::TGAImage(Buffer *_buffer)
{
    buffer = _buffer;
}

TGAImage::~TGAImage()
{
    delete buffer;
}

bool TGAImage::write_tga_file(const char *filename, bool rle)
{
    unsigned char developer_area_ref[4] = {0, 0, 0, 0};
    unsigned char extension_area_ref[4] = {0, 0, 0, 0};
    unsigned char footer[18] = {'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O', 'N', '-', 'X', 'F', 'I', 'L', 'E', '.', '\0'};
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open())
    {
        std::cerr << "can't open file " << filename << "\n";
        out.close();
        return false;
    }
    TGA_Header header;
    memset((void *)&header, 0, sizeof(header));
    header.bitsperpixel = buffer->get_channels() << 3;
    header.width = buffer->get_width();
    header.height = buffer->get_height();
    header.datatypecode = (buffer->get_channels() == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));
    header.imagedescriptor = 0x20; // top-left origin
    out.write((char *)&header, sizeof(header));
    if (!out.good())
    {
        out.close();
        std::cerr << "can't dump the tga file\n";
        return false;
    }
    if (!rle)
    {
        out.write((char *)(buffer->get()), buffer->get_width() * buffer->get_height() * buffer->get_channels());
        if (!out.good())
        {
            std::cerr << "can't unload raw data\n";
            out.close();
            return false;
        }
    }
    else
    {
        if (!unload_rle_data(out))
        {
            out.close();
            std::cerr << "can't unload rle data\n";
            return false;
        }
    }
    out.write((char *)developer_area_ref, sizeof(developer_area_ref));
    if (!out.good())
    {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }
    out.write((char *)extension_area_ref, sizeof(extension_area_ref));
    if (!out.good())
    {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }
    out.write((char *)footer, sizeof(footer));
    if (!out.good())
    {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }
    out.close();
    return true;
}

// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
bool TGAImage::unload_rle_data(std::ofstream &out)
{
    const unsigned char max_chunk_length = 128;
    unsigned long npixels = buffer->get_width() * buffer->get_height();
    unsigned long curpix = 0;
    while (curpix < npixels)
    {
        unsigned long chunkstart = curpix * buffer->get_channels();
        unsigned long curbyte = curpix * buffer->get_channels();
        unsigned char run_length = 1;
        bool raw = true;
        while (curpix + run_length < npixels && run_length < max_chunk_length)
        {
            bool succ_eq = true;
            for (int t = 0; succ_eq && t < buffer->get_channels(); t++)
            {
                succ_eq = (buffer->get()[curbyte + t] == buffer->get()[curbyte + t + buffer->get_channels()]);
            }
            curbyte += buffer->get_channels();
            if (1 == run_length)
            {
                raw = !succ_eq;
            }
            if (raw && succ_eq)
            {
                run_length--;
                break;
            }
            if (!raw && !succ_eq)
            {
                break;
            }
            run_length++;
        }
        curpix += run_length;
        out.put(raw ? run_length - 1 : run_length + 127);
        if (!out.good())
        {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
        out.write((char *)(buffer->get() + chunkstart), (raw ? run_length * buffer->get_channels() : buffer->get_channels()));
        if (!out.good())
        {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
    }
    return true;
}

Buffer *TGAImage::get_buffer()
{
    return buffer;
};