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


// template <typename T>

// IS NEVER THE OWNER OF A BUFFER
struct SWRend
{
    // T* buf; // buffer
    uint32_t* buf; // buffer
    int w;
    int h;

    SWRend(uint32_t* _buf, int _w, int _h)
    {
        buf = _buf;
        w = _w;
        h = _h;
    }

    constexpr int size() { return w * h; }
    constexpr int stride() { return w * depth_byte(); }
    constexpr int depth_byte() { return sizeof(uint32_t); }
    constexpr int depth_bit() { return sizeof(uint32_t) * 8; }


    /** Checks bounds before access. If outside of bounds, it will return the first pixel in bitmap. */
    uint32_t* operator[](int _x, int _y)
    {
        if(_x >= w || _y >= h)
        {
            Print::ln("ERROR: accessing pixel outside bounds of bitmap.");
            return buf;
        }

        return buf+_y*w+_x;
    }

    bool is_in_bounds(i2 _p)
    {
        if( _p.x > w || _p.y > h)
            return false;
        
        return true;
    }

    void draw_point(i2 _p, uint32_t _px);

    
};