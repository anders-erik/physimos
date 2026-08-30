
#include <cstdint>

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
            Str::UI(r),
            ", ",
            Str::UI(g),
            ", ",
            Str::UI(b),
        };
    }
};

typedef Arr<Pixel> Col;

/** 24-bit pixel bitmap */
class Bitmap
{
    Arr<Col> cols;

    uint height = 0;
    uint width = 0;

public:

    Bitmap(uint _width, uint _height)
    {
        height = _height;
        width = _width;

        cols.clear();

        for(uint i = 0; i < width; i++)
        {
            cols.push_back({});
            cols[i].set({}, height);
        }
    }

    // Returns a column of pixels at x=col_index
    Col& operator[](uint _col_index)
    {
        return cols[_col_index];
    }

    void clear(Pixel _pixel)
    {
        for(uint x = 0; x < width; x++)
        {
            for(uint y = 0; y < height; y++)
            {
                (*this)[x][y] = _pixel;
            }
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

    pixel_str = bmp[0][0].to_str();
    Print::ln(pixel_str);

    bmp[0][0] = {255, 255, 255};
    pixel_str = bmp[0][0].to_str();
    Print::ln(pixel_str);
    

    Print::ln("Bye, from main_swrend.cc. \n");
    return 0;
}