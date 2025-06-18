#pragma once

#include "math/vecmat.hh"



class Color {
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
};

