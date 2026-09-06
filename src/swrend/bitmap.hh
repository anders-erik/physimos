#pragma once



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
};

typedef Arr<Pixel> Col;

/** 24-bit pixel bitmap */
class Bitmap
{
    // Arr<Col> cols;
    Vec<uint8_t> data;

    uint width = 0;
    uint height = 0;

public:

    Bitmap(uint _width, uint _height)
    {
        height = _height;
        width = _width;

        data.set_size(height*width*3);
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
    
    uint8_t* get_data_mut()
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

    uint stride()
    {
        return 3 * width;
    }

    uint get_pixel_index(uint _x, uint _y)
    {
        return _y * stride() + _x*3;
    }

    // Copies the passed bitmap onto this bitmap. The lower left corner of the passed bitmap is placed at the provided x/y values
    void copy(Bitmap& _bmp_to_copy, u2 _offset)
    {
        uint x_start = _offset.x;
        uint x_end = x_start + _bmp_to_copy.w();
        uint y_start = _offset.y;
        uint y_end = y_start + _bmp_to_copy.h();

        for(uint x = 0; x < _bmp_to_copy.w(); x++)
        {
            for(uint y = 0; y < _bmp_to_copy.h(); y++)
            {
                uint x_this = x + _offset.x;
                uint y_this = y + _offset.y;

                (*this)[x_this, y_this] = _bmp_to_copy[x, y];
            }
        }
    }

    /** Checks bounds before access. If outside of bounds, it will return the first pixel in bitmap. */
    Pixel& operator[](uint _x, uint _y)
    {
        if(_x >= width || _y >= height)
        {
            Print::ln("ERROR: accessing pixel outside bounds of bitmap.");
            return (Pixel&) *(data.data_mut());
        }

        return (Pixel&) *(data.data_mut() + get_pixel_index(_x, _y));
    }

    Pixel& operator[](u2 _p)
    {
        if( _p.x >= width || _p.y >= height)
        {
            Print::ln("ERROR: accessing pixel outside bounds of bitmap.");
            return (Pixel&) *(data.data_mut());
        }

        return (Pixel&) *(data.data_mut() + get_pixel_index(_p.x, _p.y));
    }

    void clear(Pixel _pixel)
    {
        for(uint p = 0; p < data.size(); p = p + 3)
        {
            data[p] = _pixel.r;
            data[p+1] = _pixel.g;
            data[p+2] = _pixel.b;
        }
    }

    void clear(uint8_t _byte)
    {
        for(uint i = 0; i < count_bytes(); i++)
        {
            data[i] = _byte;
        }
    }

    constexpr uint count_pixels()
    {
        return height * width;
    }
    constexpr uint count_bytes()
    {
        return height * width * 3;
    }
};