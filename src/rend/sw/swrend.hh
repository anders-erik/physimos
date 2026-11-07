#pragma once

#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "lib/str.hh"
#include "lib/arr.hh"
#include "math/vec.hh"
#include "math/vecmat.hh"

#include "bitmap.hh"

#include "lib/pixel_buffer.hh"



namespace SWR
{


struct Renderer
{
    PixelBuffer buffer;

    Renderer() {}
    Renderer(PixelBuffer _buffer) : buffer {_buffer} {}

    void set_buffer(PixelBuffer _buffer)
    {
        buffer = _buffer;
    }

    void clear(PX32 _px);

    void draw_point(i2 _p, PX32 _px);
    void draw_line(i2 _p1, i2 _p2, PX32 _px);
    void draw_rectangle(i2 _p1, i2 _p2, PX32 _px);
    void draw_triangle_no_fill(i2 _p1, i2 _p2, i2 _p3, PX32 _px);
    void draw_triangle(i2 _p1, i2 _p2, i2 _p3, PX32 _px);

    void draw_line_float_lerp(f2 _p1, f2 _p2, PX32 _px);
    void parallelogram(f2 p, float a, float b, float h, PX32 color);
    void square(f2 p, float l, PX32 color);
    void rectangle(f2 p, float w, float h, PX32 color);

    void paste_bitmap(Bitmap& _bmp, i2 _pos);
    void paste_bitmap_with_mask(Bitmap& _bmp, i2 _bmp_pos, i2 _mask_pos, i2 _mask_size);
};

};