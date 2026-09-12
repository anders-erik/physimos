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


namespace SWR
{

// template <typename T>

// IS NEVER THE OWNER OF the buffer being rendered to
struct Buf
{
    // Does the first pixel in the buffer is refer to the top or the bottom of the window?
    enum YZero {
        Top, 
        Bottom
    } y_zero = Bottom;

    // T* buf; // buffer
    PX32* buf; // buffer
    int w;
    int h;

    PX32F pixel_format = PX32F::RGBA;

    int bytes_per_pixel = sizeof(PX32);

    // Buf(PX32* _buf, int _w, int _h)
    // {
    //     buf = _buf;
    //     w = _w;
    //     h = _h;
    // }

    Buf(PX32* _buf, int _w, int _h, PX32F _pixel_format, YZero _y_zero)
    {
        buf = _buf;
        w = _w;
        h = _h;
        pixel_format = _pixel_format;
        y_zero = _y_zero;
    }

    constexpr int size() { return w * h; }
    constexpr int stride() { return w * depth_byte(); }
    constexpr int depth_byte() { return sizeof(PX32); }
    constexpr int depth_bit() { return sizeof(PX32) * 8; }

    PX32 RGBA_to_underlying_format(PX32 _px_rgba)
    {
        if(pixel_format == PX32F::ARGB)
            return PX::RGBA_to_ARGB(_px_rgba);

        return _px_rgba;
    }

    /** Checks bounds before access. If outside of bounds, it will return the first pixel in bitmap. 
        The accesing coordinates are always buffer-independent: this method will flip the y-axis to keep a consistent access-coordinate system.
        TODO: turn this access into a fast, unsafe access method. Move bounds checking access to .at()!
    */
    PX32* operator[](int _x, int _y)
    {
        if(_x >= w || _y >= h)
        {
            Print::ln("ERROR: accessing pixel outside bounds of bitmap.");
            return buf;
        }

        int offset;

        if(y_zero == Bottom)
            offset = _y*w + _x;
        else
            offset = (h-_y-1)*w + _x;

        return buf + offset;
    }

    bool is_in_bounds(i2 _p)
    {
        if( _p.x > w || _p.y > h)
            return false;
        
        return true;
    }

    void draw_point(i2 _p, PX32 _px);
    void clear(PX32 _px);

    int count_pixels() {return h*w;}
    int count_bytes() {return count_pixels() * bytes_per_pixel;}

    // Bitmap interface
    void paste(Bitmap& _bmp, i2 _pos);
};

};