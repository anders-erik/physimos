#pragma once

#include "math/vecmat.hh"

using ColorInt = int;
using Colorf3  = f3;


class Color 
{
    using uint = unsigned int;
    using uchar = unsigned char;

public:

    unsigned int color = 0x000000ff;

    Color() = default;
    Color(unsigned int new_color) : color {new_color} {};

    unsigned char R() {return (uchar) (color << 24); };
    unsigned char G() {return (uchar) (color << 16); };
    unsigned char B() {return (uchar) (color << 8 ); };
    unsigned char A() {return (uchar) (color << 0 ); };


    static f4 uint_to_f4(unsigned int color_uint)
    {
        float A = ((float) ((color_uint << 24) >> 24)) / 256.0f;
        float B = ((float) ((color_uint << 16) >> 24)) / 256.0f;
        float G = ((float) ((color_uint <<  8) >> 24)) / 256.0f;
        float R = ((float) ((color_uint <<  0) >> 24)) / 256.0f;

        return {R, G, B, A};
    }

    static unsigned int f4_to_uint(f4 color_f4)
    {
        using uint = unsigned int;

        uint R = ((uint) (color_f4.x * 255.0f)) << 24;
        uint G = ((uint) (color_f4.y * 255.0f)) << 16;
        uint B = ((uint) (color_f4.z * 255.0f)) << 8;
        uint A = ((uint) (color_f4.w * 255.0f)) << 0;

        return R + G + B + A;
    }


    static Colorf3 int_to_f3(ColorInt color_int)
    {
        Colorf3 ret_f3;

        ret_f3.x = (float) ((uchar) (color_int >> 16)) /  256.0f;
        ret_f3.y = (float) ((uchar) (color_int >> 8 )) /  256.0f;
        ret_f3.z = (float) ((uchar) (color_int >> 0 )) /  256.0f;

        return ret_f3;
    }

    static ColorInt f3_to_int(Colorf3 color_f3)
    {
        ColorInt ret_int = 0x00000000;

        ret_int += ((ColorInt) (color_f3.x * 255.0f)) << 16;
        ret_int += ((ColorInt) (color_f3.y * 255.0f)) << 8;
        ret_int += ((ColorInt) (color_f3.z * 255.0f));

        return ret_int;
    }
};

