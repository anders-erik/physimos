
#include <cstdint>
#include <cstring>

#include "lib/print.hh"
#include "lib/arr.hh"

#include "math/vec.hh"

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

    uint height = 0;
    uint width = 0;

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
        return _y * stride() + _x;
    }

    Pixel& operator[](uint _x, uint _y)
    {
    
        return (Pixel&) *(data.data_mut() + get_pixel_index(_x, _y));
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
        for(uint i = 0; i < width; i++)
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

int main(int argc, const char** argv)
{
    Print::ln("Hello from main_swrend.cc");

    Bitmap bmp {2, 1};

    bmp.clear({1, 2, 3});

    Str pixel_str;

    pixel_str = bmp[0, 0].to_str();
    Print::ln(pixel_str);

    bmp[0, 0] = {255, 255, 255};
    pixel_str = bmp[0, 0].to_str();
    Print::ln(pixel_str);

    bmp[0, 0] = {255, 255, 255};
    Print::ln(bmp[0, 0].to_str_hex());
    bmp[0, 0] = {0, 100, 200};
    Print::ln(bmp[0, 0].to_str_hex());
    

    Print::ln("Bye, from main_swrend.cc. \n");
    return 0;
}