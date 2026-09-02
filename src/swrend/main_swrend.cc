
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "lib/print.hh"
#include "lib/arr.hh"
#include "lib/file.hh"

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

    uint h() const {return height;}
    uint w() const {return width;}
    
    uint8_t* get_data_mut()
    {
        return data.data_mut();
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


int file_echo(const char* _file_path_c, void* data, uint _data_length)
{
    int fd = open(_file_path_c, O_WRONLY | O_CREAT );
    if(fd < 0)
    {
        Print::ln("Error: Failed to open file for echo function.");
        return -1;
    }
    fchmod(fd, 0777);


    int write_ret = write(fd, data, _data_length);
    if(write_ret < 0)
    {
        Print::ln("Error: Failed to write to file in echo function.");
        return -1;
    }


    fd = close(fd);
    if(fd < 0)
    {
        Print::ln("Error: Failed to close file in echo function.");
        return -1;
    }

    return 0;
}


struct BMPFileInfoHeader
{
    Vec<uint8_t> buff {14};

    uint file_info_header_size = 14;


    BMPFileInfoHeader(const Bitmap& _bitmap)
    {
        set_BM();
        set_file_size(_bitmap);
        set_reserved_1();
        set_reserved_2();
        set_first_pixel_location();
    }

    void set_BM()
    {
        buff[0] = 'B';
        buff[1] = 'M';
    }
    Vec<uint8_t> get_BM()
    {
        return Vec<uint8_t>(2, (buff.data_mut()+0));
    }

    void set_file_size(const Bitmap& _bitmap)
    {
        uint file_size = 54 + get_padded_BPM_stride(_bitmap) * _bitmap.h();
        buff[2] = (file_size & 0x000000FF) >> 0;
        buff[3] = (file_size & 0x0000FF00) >> 8;
        buff[4] = (file_size & 0x00FF0000) >> 16;
        buff[5] = (file_size & 0xFF000000) >> 24;
    }
    Vec<uint8_t> get_file_size()
    {
        return Vec<uint8_t>(4, (buff.data_mut()+2));
    }

    void set_reserved_1()
    {
        buff[6] = 0x00;
        buff[7] = 0x00;
    }
    Vec<uint8_t> get_reserved_1()
    {
        return Vec<uint8_t>(2, (buff.data_mut()+6));
    }

    void set_reserved_2()
    {
        buff[8] = 0x00;
        buff[9] = 0x00;
    }
    Vec<uint8_t> get_reserved_2()
    {
        return Vec<uint8_t>(2, (buff.data_mut()+8));
    }

    void set_first_pixel_location()
    {
        uint first_pixel_location = 54;
        buff[10] = (first_pixel_location & 0x000000FF) >> 0;
        buff[11] = (first_pixel_location & 0x0000FF00) >> 8;
        buff[12] = (first_pixel_location & 0x00FF0000) >> 16;
        buff[13] = (first_pixel_location & 0xFF000000) >> 24;
    }
    Vec<uint8_t> get_first_pixel_location()
    {
        return Vec<uint8_t>(4, (buff.data_mut()+10));
    }



    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap)
    {
        uint stride_byte_count = _bitmap.w() * 3;
        uint padding_count = 4 - (stride_byte_count % 4);
        return stride_byte_count + padding_count;
    }


    // bool operator!=(const BMPFileInfoHeader& rhs) { return *this == rhs ? false : true;};
    // bool operator==(const BMPFileInfoHeader& rhs)
    // {
    //     return  BM                      == rhs.BM                   &&
    //             file_size               == rhs.file_size            &&
    //             reserved_1              == rhs.reserved_1           &&
    //             reserved_2              == rhs.reserved_2           &&
    //             first_pixel_location    == rhs.first_pixel_location;
    // }

    // Str to_str()
    // {
    //     return  Str("FileHeader: ") + Str::UI(BM) + ", " +
    //             Str::UI(file_size) + ", " +
    //             Str::UI(reserved_1) + ", " +
    //             Str::UI(reserved_2) + ", " +
    //             Str::UI(first_pixel_location);
    // }
};


class BPMIO
{
public:

    static void Export(Str _file_path, Bitmap& _bitmap)
    {
        file_echo(_file_path.to_c_str(), _bitmap.get_data_mut(), _bitmap.count_bytes());
    }

    static Bitmap Import(Str _file_path)
    {

    }
};


int main(int argc, const char** argv)
{
    Print::ln("Hello from main_swrend.cc");

    Bitmap bmp {2, 2};

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

    BPMIO::Export("tmp/2x2.bmp", bmp);


    BMPFileInfoHeader file_info_header {bmp};
    // auto vec = file_info_header.get_BM();
    

    Print::ln("Bye, from main_swrend.cc. \n");
    return 0;
}