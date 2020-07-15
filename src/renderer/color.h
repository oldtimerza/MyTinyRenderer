#ifndef __TGA_COLOR_H__
#define __TGA_COLOR_H__

struct TGAColor
{
    union {
        struct
        {
            unsigned char b, g, r, a;
        };
        unsigned char raw[4];
        unsigned int val;
    };
    int bytespp;

    TGAColor() : val(0), bytespp(1)
    {
    }

    TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4)
    {
    }

    TGAColor(int v, int bpp) : val(v), bytespp(bpp)
    {
    }

    TGAColor(const TGAColor &c) : val(c.val), bytespp(c.bytespp)
    {
    }

    TGAColor(const unsigned char *p, int bpp) : val(0), bytespp(bpp)
    {
        for (int i = 0; i < bpp; i++)
        {
            raw[i] = p[i];
        }
    }

    TGAColor &operator=(const TGAColor &c)
    {
        if (this != &c)
        {
            bytespp = c.bytespp;
            val = c.val;
        }
        return *this;
    }
};

#endif