#pragma once


#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "lib/arr.hh"
#include "lib/print.hh"
#include "lib/str.hh"
#include "lib/pixel_buffer.hh"

#include "math/vec.hh"
#include "math/vecmat.hh"


// typedef uint32_t PX32RGBA;
typedef uint32_t PX32;

/** Logical ordering if pixel bytes when manipulated in-code. Actual layout is reversed in little-endian memory. */
enum class PX32F
{
    RGBA,
    ARGB,
};

struct PX
{
    static inline PX32 RGBA_to_ARGB(PX32 _px)
    {
        PX32 A___ = (_px & 0x000000FF) << 24;
        PX32 _RGB = (_px & 0xFFFFFF00) >> 8;
        return (A___ | _RGB);
    }

    static inline PX32 ARGB_to_RGBA(PX32 _px)
    {
        PX32 ___A = (_px & 0xFF000000) >> 24;
        PX32 RGB_ = (_px & 0x00FFFFFF) << 8;
        return (___A | RGB_);
    }

    static inline uint8_t R(PX32 _px) { return (uint8_t)(_px >> 24); }
    static inline uint8_t G(PX32 _px) { return (uint8_t)(_px >> 16); }
    static inline uint8_t B(PX32 _px) { return (uint8_t)(_px >> 8 ); }
    static inline uint8_t A(PX32 _px) { return (uint8_t)(_px >> 0 ); }

    static Str to_str(PX32 _px)
    {
        return {
            "(",
            Str::UI(PX::R(_px)),
            ", ",
            Str::UI(PX::G(_px)),
            ", ",
            Str::UI(PX::B(_px)),
            ", ",
            Str::UI(PX::A(_px)),
            ")",
        };
    }

    static Str to_str_hex(PX32 _px)
    {
        char hex[16];
        memset(hex, 0, 16);

        sprintf(hex, "%x", _px);
    
        return {
            Str(hex),
        };
    }
};



struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    constexpr
    Pixel() : r {0}, g {0}, b {0} 
    {
    }
    Pixel(uint8_t _r, uint8_t _g, uint8_t _b) : r {_r}, g {_g}, b {_b} 
    {
    }

    Pixel& operator=(const Pixel& rhs)
    {
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        return *this;
    }

    Str to_str()
    {
        return {
            "(",
            Str::UI(r),
            ", ",
            Str::UI(g),
            ", ",
            Str::UI(b),
            ")",
        };
    }
    Str to_str_hex()
    {
        int  num = r << 24;
        num += g << 16;
        num += b << 8;

        char hex[16];
        memset(hex, 0, 16);

        sprintf(hex, "%x", num);
    
        return {
            Str(hex),
        };
    }

    PX32 to_PX32RGBA()
    {
        PX32 ret_px = 0;

        ret_px += r << 24;
        ret_px += g << 16;
        ret_px += b << 8 ;
        ret_px += 0x000000FF ;

        return ret_px;
    }
};



// IS NEVER THE OWNER OF the buffer being rendered to
struct PixelBuffer
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

    PixelBuffer() {}
    PixelBuffer(PX32* _buf, int _w, int _h, PX32F _pixel_format, YZero _y_zero)
    {
        set(_buf, _w, _h, _pixel_format, _y_zero);
    }

    void set(PX32* _buf, int _w, int _h, PX32F _pixel_format, YZero _y_zero)
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

    
    int count_pixels() {return h*w;}
    int count_bytes() {return count_pixels() * bytes_per_pixel;}

};

