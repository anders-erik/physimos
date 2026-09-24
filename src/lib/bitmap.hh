#pragma once

#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "lib/arr.hh"
#include "lib/print.hh"
#include "lib/str.hh"

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

/** 
    Handles 6 scenarios of the pasting of a 1D span/size to the position of a 0-origined 1D destination span.
    See media/1D_paste_box.png for visual description
*/
struct PasteBox1D
{
    int size_intersection; // size of the box resulting from logical intersection operation

    int origin_dest; // location where the paste begins in the destination bitmap (the bitmap onto wich we are pasting)
    int origin_src;  // location of the sampling origin in the source bitmap (the bitmap from which we are copying data)



    void set_box(int size_dest, int size_src, int pos_src)
    {

        bool src_is_outside_dest_to_the_left = (pos_src + size_src) < 0;
        bool src_is_outside_dest_to_the_right = pos_src > size_dest;

        if(src_is_outside_dest_to_the_left || src_is_outside_dest_to_the_right)
        {
            size_intersection = 0;
            return;
        }
        
        // intersection guaranteed!

        bool src_start_at_negative_pos = pos_src < 0;
        // bool src_start_at_positive_pos = !src_start_at_negative_pos;

        if(src_start_at_negative_pos)
        {
            bool src_intersect_both_left_and_right_of_dest = pos_src + size_src > size_dest;

            if(src_intersect_both_left_and_right_of_dest)
            {
                size_intersection = size_dest;
                origin_dest = 0;
                origin_src = -pos_src;
            }
            else
            {
                size_intersection = pos_src + size_src;
                origin_dest = 0;
                origin_src = -pos_src;
            }
        }
        else // src is pasted at location within the size of the destination box
        {
            bool src_is_contained_within_dest = pos_src + size_src < size_dest;

            if(src_is_contained_within_dest)
            {
                size_intersection = size_src;
                origin_dest = pos_src;
                origin_src = 0;
            }
            else // paste intersects only right side of destination box
            {
                size_intersection = size_dest - pos_src;
                origin_dest = pos_src;
                origin_src = 0;
            }
        }

    }

private:


    void set_intersection()
    {
        
    }
};


// typedef Arr<Pixel> Col;

/** 32-bit pixel bitmap */
class Bitmap
{
    Vec<PX32> data;

    uint width = 0;
    uint height = 0;

    PX32F format = PX32F::RGBA; // Pixel format

public:

    Bitmap(uint _width, uint _height)
    {
        allocate(_width, _height);

        clear(0x00000000);
    }

    Bitmap(uint _width, uint _height, PX32F _format)
    {
        allocate(_width, _height);

        format = _format;

        clear(0x00000000);
    }

    Bitmap& allocate(uint _width, uint _height)
    {
        height = _height;
        width = _width;
        data.set_size(height*width);

        return *this;
    }

    uint h() const {return height;}
    uint w() const {return width;}
    uint bytes_per_pixel() const {return sizeof(PX32);}
    
    PX32* get_data_mut()
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

    PX32F get_format() { return format; }
    /** Updates the format-value AND move all data bits to match the bit-format while aligning all R-G-B-A-values. */
    void set_format(PX32F _new_format)
    {
        if(format == _new_format)
            return;

        // RGBA -> ARGB
        if(format == PX32F::RGBA && _new_format == PX32F::ARGB)
        {
            for(uint i = 0; i < count_pixels(); i++)
            {
                PX32 A___ = (data[i] & 0x000000FF) << 24;
                PX32 _RGB = (data[i] & 0xFFFFFF00) >> 8;
                data[i] = (A___ | _RGB);
            }
        }

        // ARGB -> RGBA 
        if(format == PX32F::ARGB && _new_format == PX32F::RGBA)
        {
            for(uint i = 0; i < count_pixels(); i++)
            {
                PX32 ___A = (data[i] & 0xFF000000) >> 24;
                PX32 RGB_ = (data[i] & 0x00FFFFFF) << 8;
                data[i] = (___A | RGB_);
            }
        }

        format = _new_format;
    }

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


    void paste2(Bitmap& _bmp_to_paste, i2 _offset)
    {
        PasteBox1D paste_box_x;
        paste_box_x.set_box((*this).width, _bmp_to_paste.width, _offset.x);
        PasteBox1D paste_box_y;
        paste_box_y.set_box((*this).height, _bmp_to_paste.height, _offset.y);

        for(uint x = 0; x < paste_box_x.size_intersection; x++)
        {
            for(uint y = 0; y < paste_box_y.size_intersection; y++)
            {
                int dest_x = paste_box_x.origin_dest + x;
                int dest_y = paste_box_y.origin_dest + y;
                int src_x = paste_box_x.origin_src + x;
                int src_y = paste_box_y.origin_src + y;

                (*this)[dest_x, dest_y] = _bmp_to_paste[src_x, src_y];
            }
        }
    }

    // Copies the passed bitmap onto this bitmap. The lower left corner of the passed bitmap is placed at the provided x/y values
    // NOTE: pasting will fail if the pasted bitmap is not fully contained in the current bitmap
    void paste(Bitmap& _bmp_to_paste, i2 _offset)
    {
        // Intersection box check
        int H = (int)height;
        int W = (int)width;
        int h = (int)_bmp_to_paste.height;
        int w = (int)_bmp_to_paste.width;
        
        bool pasted_bitmap_is_above_current_bitmap = _offset.y > H;
        bool pasted_bitmap_is_below_current_bitmap = (_offset.y + h) < 0;
        bool pasted_bitmap_is_to_the_right_of_current_bitmap = _offset.x > W;
        bool pasted_bitmap_is_to_the_left_of_current_bitmap = (_offset.x + w) < 0;

        bool pasted_bitmap_not_in_current_bitmap =  pasted_bitmap_is_above_current_bitmap ||
                                                    pasted_bitmap_is_below_current_bitmap ||
                                                    pasted_bitmap_is_to_the_right_of_current_bitmap ||
                                                    pasted_bitmap_is_to_the_left_of_current_bitmap;

        bool pasted_bitmap_is_fully_contained_in_current_bitmap =   _offset.x > 0 && 
                                                                    _offset.y > 0 &&
                                                                    _offset.x + w <= W &&
                                                                    _offset.y + h <= H;

        u2 size_to_paste;
        u2 offset_to_paste;

        if(!pasted_bitmap_is_fully_contained_in_current_bitmap)
        {
            if(pasted_bitmap_not_in_current_bitmap)
                return;

            // TODO: DO PARTIAL INTERSECTION TESTS HERE!
            
            // X Checks
            bool offset_x_to_paste_starts_at_0 = _offset.x < 0; // since we've already checked if pasted is whole to the left of the current bitmap
            offset_to_paste.x = offset_x_to_paste_starts_at_0 ? 0 : _offset.x;
            bool size_to_paste_exceeds_W = (offset_to_paste.x + _bmp_to_paste.width) > W;
            size_to_paste.x = size_to_paste_exceeds_W ? W - offset_to_paste.x : _bmp_to_paste.width;

            // Y Checks
            bool offset_y_to_paste_starts_at_0 = _offset.y < 0;
            offset_to_paste.y = offset_y_to_paste_starts_at_0 ? 0 : _offset.y;
            bool size_to_paste_exceeds_H = (offset_to_paste.y + _bmp_to_paste.height) > H;
            size_to_paste.y = size_to_paste_exceeds_H ? H - offset_to_paste.y : _bmp_to_paste.height;

        }
        else
        {
            size_to_paste.x = _bmp_to_paste.width;
            size_to_paste.y = _bmp_to_paste.height;
            offset_to_paste.x = _offset.x;
            offset_to_paste.y = _offset.y;
        }

        // if(_offset.x > width || _offset.y >

        for(uint x = 0; x < size_to_paste.x; x++)
        {
            for(uint y = 0; y < size_to_paste.y; y++)
            {
                uint x_this = x + offset_to_paste.x;
                uint y_this = y + offset_to_paste.y;

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
    PX32& operator[](uint _x, uint _y)
    {
        if(_x >= width || _y >= height)
        {
            Print::ln("ERROR: accessing pixel outside bounds of bitmap.");
            return (PX32&) *(data.data_mut());
        }

        return (PX32&) *(data.data_mut() + get_pixel_index(_x, _y));
    }

    PX32& operator[](u2 _p)
    {
        if( _p.x >= width || _p.y >= height)
        {
            Print::ln("ERROR: accessing pixel outside bounds of bitmap.");
            return (PX32&) *(data.data_mut());
        }

        return (PX32&) *(data.data_mut() + get_pixel_index(_p.x, _p.y));
    }

    /** Does NOT check the current pixel format, but will set the provided Pixel data directly as recieved. */
    void clear(PX32 _pixel)
    {
        for(uint p = 0; p < count_pixels(); p++)
            data[p] = _pixel;
    }

    // A call to clear the whole bitmap using the provided pixel in RGBA format.
    // If the bitmap is currently configured for another pixel format, then the pixel data is adjusted before writing to the bitmap buffer.
    void clear_RGBA(PX32 _rgba_pixel)
    {
        PX32 bit_adjusted_px;

        if(format == PX32F::RGBA)
            bit_adjusted_px = _rgba_pixel;
        else if(format == PX32F::ARGB)
            bit_adjusted_px = PX::RGBA_to_ARGB(_rgba_pixel);

        for(uint p = 0; p < count_pixels(); p++)
            data[p] = bit_adjusted_px;
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
        return height * width * sizeof(PX32);
    }
};