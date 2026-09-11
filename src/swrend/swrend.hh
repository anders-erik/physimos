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
    enum YZero {
        Top, 
        Bottom
    } y_zero = Bottom;

    // T* buf; // buffer
    uint32_t* buf; // buffer
    int w;
    int h;

    int bytes_per_pixel = sizeof(uint32_t);

    Buf(uint32_t* _buf, int _w, int _h)
    {
        buf = _buf;
        w = _w;
        h = _h;
    }

    constexpr int size() { return w * h; }
    constexpr int stride() { return w * depth_byte(); }
    constexpr int depth_byte() { return sizeof(uint32_t); }
    constexpr int depth_bit() { return sizeof(uint32_t) * 8; }


    /** Checks bounds before access. If outside of bounds, it will return the first pixel in bitmap. 
        The accesing coordinates are always buffer-independent: this method will flip the y-axis to keep a consistent access-coordinate system.
        TODO: turn this access into a fast, unsafe access method. Move bounds checking access to .at()!
    */
    uint32_t* operator[](int _x, int _y)
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

    void draw_point(i2 _p, uint32_t _px);
    void clear(uint32_t _px);

    int count_pixels() {return h*w;}
    int count_bytes() {return count_pixels() * bytes_per_pixel;}

    // Bitmap interface
    void paste(Bitmap& _bmp, i2 _pos);
};

};