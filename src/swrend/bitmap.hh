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

typedef uint32_t PX32RGBA;
typedef uint32_t PX32;
enum class PX32F
{
    RGBA,
};

struct PX
{
    static inline uint8_t R(PX32RGBA _px) { return (uint8_t)(_px >> 24); }
    static inline uint8_t G(PX32RGBA _px) { return (uint8_t)(_px >> 16); }
    static inline uint8_t B(PX32RGBA _px) { return (uint8_t)(_px >> 8 ); }
    static inline uint8_t A(PX32RGBA _px) { return (uint8_t)(_px >> 0 ); }

    static Str to_str(PX32RGBA _px)
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

    static Str to_str_hex(PX32RGBA _px)
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

    PX32RGBA to_PX32RGBA()
    {
        PX32RGBA ret_px = 0;

        ret_px += r << 24;
        ret_px += g << 16;
        ret_px += b << 8 ;
        ret_px += 0x000000FF ;

        return ret_px;
    }
};

// typedef Arr<Pixel> Col;

/** 24-bit pixel bitmap */
class Bitmap
{
    // Arr<Col> cols;
    Vec<PX32RGBA> data;

    uint width = 0;
    uint height = 0;

    PX32F format = PX32F::RGBA;

public:

    Bitmap(uint _width, uint _height)
    {
        height = _height;
        width = _width;

        data.set_size(height*width);
        // data.set(0);
        clear(0);

        // for(uint i = 0; i < width; i++)
        // {
        //     cols.push_back({});
        //     cols[i].set({}, height);
        // }
    }

    uint h() const {return height;}
    uint w() const {return width;}
    uint bytes_per_pixel() const {return sizeof(PX32RGBA);}
    
    PX32RGBA* get_data_mut()
    {
        return data.data_mut();
    }

    bool is_in_bounds(u2 _p)
    {
        if( _p.x > width || _p.y > height)
            return false;
        
        return true;
    }

    // Returns a column of pixels at x=col_index
    // Col& operator[](uint _col_index)
    // {
    //     return cols[_col_index];
    // }

    uint stride_byte()
    {
        return width * 4;
    }

    uint get_pixel_index(uint _x, uint _y)
    {
        return _y * w() + _x;
    }

    Bitmap get_subbitmap(u2 _pos, u2 _size)
    {
        Bitmap bitmap {_size.x, _size.y};

        for(uint x = 0; x < bitmap.w(); x++)
        {
            for(uint y = 0; y < bitmap.h(); y++)
            {
                bitmap[x,y] = (*this)[x+_pos.x, y+_pos.y];
            }
        }

        return bitmap;
    }

    // Copies the passed bitmap onto this bitmap. The lower left corner of the passed bitmap is placed at the provided x/y values
    // NOTE: pasting will fail if the pasted bitmap is not fully contained in the current bitmap
    void paste(Bitmap& _bmp_to_paste, u2 _offset)
    {
        for(uint x = 0; x < _bmp_to_paste.w(); x++)
        {
            for(uint y = 0; y < _bmp_to_paste.h(); y++)
            {
                uint x_this = x + _offset.x;
                uint y_this = y + _offset.y;

                (*this)[x_this, y_this] = _bmp_to_paste[x, y];
            }
        }
    }

    // Returns a cropped bitmap equal to the intersection of the two bitmaps. 
    // The position parameters is the location of the bitmap to crop.
    static Bitmap intersection(Bitmap& _target, Bitmap& _bmp_to_crop, u2 _pos)
    {
        uint new_width;
        uint new_height;

        if(_pos.x > _target.w()) // outside target bitmap dimensions
        {
            new_width = 0;
        }
        else if( _target.w() < (_bmp_to_crop.w() + _pos.x)) // crop
        {
            // Need to crop width
            new_width = _target.w() - _pos.x;
        }
        else // no need to crop
        {
            new_width = _bmp_to_crop.w();
        }


        if(_pos.y > _target.h()) // outside target bitmap dimensions
        {
            new_height = 0;
        }
        else if(_target.h() < (_bmp_to_crop.h() + _pos.y))
        {
            // Need to crop height
            new_height = _target.h() - _pos.y;
        }
        else
        {
            new_height = _bmp_to_crop.h();
        }


        if(new_width == 0 || new_height == 0)
            return {0, 0};

        Bitmap bmp {new_width, new_height};

        for(uint x = 0; x < bmp.w(); x++)
        {
            for(uint y = 0; y < bmp.h(); y++)
            {
                bmp[x, y] = _bmp_to_crop[x, y];
            }
        }

        return bmp;
    }

    Bitmap scale(double _scaling_factor)
    {
        uint new_width = (uint) ((double)width * _scaling_factor);
        uint new_height = (uint) ((double)height * _scaling_factor);
        
        Bitmap bmp {new_width, new_height};

        for(uint x = 0; x < bmp.w(); x++)
        {
            for(uint y = 0; y < bmp.h(); y++)
            {
                uint x_sample = x / _scaling_factor;
                uint y_sample = y / _scaling_factor;

                bmp[x, y] = (*this)[x_sample, y_sample];
            }
        }

        return bmp;
    }

    /** Checks bounds before access. If outside of bounds, it will return the first pixel in bitmap. */
    PX32RGBA& operator[](uint _x, uint _y)
    {
        if(_x >= width || _y >= height)
        {
            Print::ln("ERROR: accessing pixel outside bounds of bitmap.");
            return (PX32RGBA&) *(data.data_mut());
        }

        return (PX32RGBA&) *(data.data_mut() + get_pixel_index(_x, _y));
    }

    PX32RGBA& operator[](u2 _p)
    {
        if( _p.x >= width || _p.y >= height)
        {
            Print::ln("ERROR: accessing pixel outside bounds of bitmap.");
            return (PX32RGBA&) *(data.data_mut());
        }

        return (PX32RGBA&) *(data.data_mut() + get_pixel_index(_p.x, _p.y));
    }

    void clear(PX32RGBA _pixel)
    {
        // for(uint p = 0; p < data.size(); p = p + 3)
        // {
        //     data[p] = _pixel.r;
        //     data[p+1] = _pixel.g;
        //     data[p+2] = _pixel.b;
        // }
        for(uint p = 0; p < count_pixels(); p++)
            data[p] = _pixel;
    }

    // void clear(uint8_t _byte)
    // {
    //     for(uint i = 0; i < count_bytes(); i++)
    //     {
    //         data[i] = _byte;
    //     }
    // }

    constexpr uint count_pixels()
    {
        return height * width;
    }
    constexpr uint count_bytes()
    {
        return height * width * 3;
    }
};