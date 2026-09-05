
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#include "lib/print.hh"
#include "lib/arr.hh"
#include "lib/file.hh"

#include "math/vec.hh"
#include "math/vecmat.hh"



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


int file_echo(const char* _file_path_c, void* data, uint _data_length)
{
    remove(_file_path_c);

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
    Vec<uint8_t> BM {2};
    uint32_t file_size;
    Vec<uint8_t> reserved_1 {2};
    Vec<uint8_t> reserved_2 {2};
    uint32_t data_offset;

    Vec<uint8_t> buff {14};


    BMPFileInfoHeader(const Bitmap& _bitmap)
    {
        // set_BM();
        // set_file_size(_bitmap);
        // set_reserved_1();
        // set_reserved_2();
        // set_first_pixel_location();

        BM[0] = 'B';
        BM[1] = 'M';

        file_size = 54 + get_padded_BPM_stride(_bitmap) * _bitmap.h();

        reserved_1[0] = 0;
        reserved_1[1] = 0;

        reserved_2[0] = 0;
        reserved_2[1] = 0;

        data_offset = 54;
    }

    Vec<uint8_t> get_header_buff()
    {
        Vec<uint8_t> buffer {14};

        bool arch_is_big_endian = false;

        if(arch_is_big_endian)
        {
            // flip_integer_endianess
        }

        buffer[0] = BM[0];
        buffer[1] = BM[1];

        memcpy(buffer.data_mut() + 2, &file_size, 4);

        buffer[6] = reserved_1[0];
        buffer[7] = reserved_1[1];

        buffer[8] = reserved_2[0];
        buffer[9] = reserved_2[1];

        memcpy(buffer.data_mut() + 10, &data_offset, 4);

        return buffer;
    }

    void set_BM()
    {
        buff[0] = 'B';
        buff[1] = 'M';
    }
    Vec<uint8_t> get_BM_buff()
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
    Vec<uint8_t> get_file_size_buff()
    {
        return Vec<uint8_t>(4, (buff.data_mut()+2));
    }
    uint get_file_size()
    {
        uint file_size = 0;
        file_size += (buff[2] << 24);
        file_size += (buff[3] << 16);
        file_size += (buff[4] << 8 );
        file_size += (buff[5] << 0 );
        return file_size;
    }

    void set_reserved_1()
    {
        buff[6] = 0x00;
        buff[7] = 0x00;
    }
    Vec<uint8_t> get_reserved_1_buff()
    {
        return Vec<uint8_t>(2, (buff.data_mut()+6));
    }

    void set_reserved_2()
    {
        buff[8] = 0x00;
        buff[9] = 0x00;
    }
    Vec<uint8_t> get_reserved_2_buff()
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
    Vec<uint8_t> get_first_pixel_location_buff()
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

    Str to_str()
    {
        Str info_header_str;
        info_header_str += "info_header_str: \n   ";

        info_header_str += Str::CH(BM[0]);
        info_header_str += Str::CH(BM[1]) + ", "; 

        info_header_str += Str::UI(file_size)  + ", ";

        info_header_str += Str::CH(reserved_1[0]);
        info_header_str += Str::CH(reserved_1[1])  + ", ";

        info_header_str += Str::CH(reserved_2[0]);
        info_header_str += Str::CH(reserved_2[1])  + ", ";

        info_header_str += Str::UI(data_offset);

        info_header_str += "\n";

        // return  Str("FileHeader: ") + Str::UI(BM) + ", " +
        //         Str::UI(get_file_size()) + ", " +
        //         Str::UI(reserved_1) + ", " +
        //         Str::UI(reserved_2) + ", " +
        //         Str::UI(first_pixel_location);
        
        return info_header_str;
    }
};


class BMPDIBHeader
{
public:

    uint32_t DIB_bytes;
    uint32_t width_pixels;
    uint32_t height_pixels;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t data_bytes;
    uint32_t res_hori;
    uint32_t res_vert;
    uint32_t color_count;
    uint32_t important_colors;

    BMPDIBHeader(Bitmap& _bitmap)
    {
        DIB_bytes = 40;
        width_pixels = _bitmap.w();
        height_pixels = _bitmap.h();
        planes = 1;
        bits_per_pixel = 24;
        compression = 0;
        data_bytes = _bitmap.h() * get_padded_BPM_stride(_bitmap);
        res_hori = 2835;
        res_vert = 2835;
        color_count = 0;
        important_colors = 0;
    }

    Vec<uint8_t> get_DIB_buffer()
    {
        Vec<uint8_t> ret_buff {DIB_bytes};

        memcpy(ret_buff.data_mut(), this, DIB_bytes);

        return ret_buff;
    }

    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap)
    {
        uint stride_byte_count = _bitmap.w() * 3;
        uint padding_count = 4 - (stride_byte_count % 4);
        return stride_byte_count + padding_count;
    }
};


class BPMIO
{
public:

    Bitmap bitmap;
    BMPFileInfoHeader finfo_header;
    BMPDIBHeader DIB_Header;

    BPMIO(Bitmap& _bitmap)
        :   bitmap {_bitmap},
            finfo_header { BMPFileInfoHeader(bitmap) },
            DIB_Header { BMPDIBHeader(bitmap) }
    {
    }




    void Export(Str _file_path)
    {
        Vec<uint8_t> export_buff {finfo_header.file_size};

        Vec<uint8_t> info_header_buf = finfo_header.get_header_buff();
        Vec<uint8_t> DIB_header_buf = DIB_Header.get_DIB_buffer();
        Vec<uint8_t> padded_data_buf = pad_bitmap_data();

        // uint8_t* info_header_ptr = finfo_header.get_header_buff().data_mut();
        // uint8_t* DIB_header_ptr = DIB_Header.get_DIB_buffer().data_mut();
        // uint8_t* padded_data_ptr = pad_bitmap_data().data_mut();

        memcpy( export_buff.data_mut() + 0 , 
                info_header_buf.data_mut(), 
                14);
        memcpy( export_buff.data_mut() + 14, 
                DIB_header_buf.data_mut(), 
                40);
        memcpy( export_buff.data_mut() + finfo_header.data_offset, 
                padded_data_buf.data_mut(), 
                DIB_Header.data_bytes);

        file_echo(  _file_path.to_c_str(), 
                    export_buff.data_mut(), 
                    finfo_header.file_size);
    }

    Bitmap Import(Str _file_path)
    {

        return Bitmap{0,0};
    }


    Vec<uint8_t> pad_bitmap_data()
    {
        uint stride = bitmap.stride();
        uint padded_stride = get_padded_BPM_stride(bitmap);

        Vec<uint8_t> padded_data {bitmap.h() * padded_stride};
        padded_data.set(0x00);

        // uint8_t* bitmap_base_ptr = bitmap.get_data_mut();
        // uint8_t* padded_base_ptr = padded_data.data_mut();

        for(uint i = 0; i < bitmap.h(); i++)
        {
            // uint index_start_of_row = i * padded_stride;
            memcpy( padded_data.data_mut() + i * padded_stride,
                    bitmap.get_data_mut() + i * stride,
                    stride                                  );
        }

        return padded_data;
    }

    /** Returns number of bytes */
    static uint get_padded_BPM_stride(const Bitmap& _bitmap)
    {
        uint stride_byte_count = _bitmap.w() * 3;
    
        uint padding_count;

        // BMP always pads the stride to multiples of 4 bytes
        if( (stride_byte_count % 4) == 0)
            padding_count = 0;
        else
            padding_count = 4 - (stride_byte_count % 4);

        return stride_byte_count + padding_count;
    }
};


void test_bitmap_2x2()
{
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

    // BPMIO::Export("tmp/2x2.bmp", bmp);


    BMPFileInfoHeader file_info_header {bmp};
    Vec<uint8_t> header_buff = file_info_header.get_header_buff();
    // auto vec = file_info_header.get_BM();
    Print::ln(file_info_header.to_str());

    BMPDIBHeader file_DIB_header {bmp};
    Vec<uint8_t> DIB_buff = file_DIB_header.get_DIB_buffer();

    
    bmp[0, 0] = {25, 25, 25};
    bmp[1, 0] = {100, 100, 100};
    bmp[0, 1] = {150, 150, 150};
    bmp[1, 1] = {200, 200, 200};


    


    BPMIO bmp_io {bmp};
    bmp_io.Export("tmp/2x2.bmp");
}

// Str logf_path = "tmp/log.txt";
// void logf(Str _msg, )
// {

// }


// y = kx + m
class Line
{
public:

    d2 p1;
    d2 p2;

    double k;
    double m;

    Line(d2 _p1, d2 _p2) : p1 {_p1}, p2 {_p2}
    {
        // y = kx + m
        k = (p2.y - p1.y)/(p2.x - p1.x);
        m = p1.y - p1.x*k;
    }

    double operator[](double _x)
    {
        return k*_x + m;
    }

};


uint Min(uint a, uint b)
{
    return a > b ? b : a;
}

uint Max(uint a, uint b)
{
    return a > b ? a : b;
}

double orient_2d_line(d2 p, d2 p1, d2 p2)
{
    
}

class BitmapDrawer
{
public:

    Pixel pixel;

    void set_pixel_color(Pixel _pixel)
    {
        pixel = _pixel;
    }

    void point(Bitmap& _bmp, u2 _p, Pixel _px)
    {
        if(_bmp.is_in_bounds(_p))
            _bmp[_p] = _px;
        else
            Print::ln("WARN: tried drawing point outseide bitmap bounds.");
    }
    
    /** Draw pixelated line using kx + m where pixels are filled in from the gaps made from k-values greater than 2. */
    void line_kxm_1(Bitmap& _bmp, u2 _p1, u2 _p2, Pixel _px)
    {
        if(!_bmp.is_in_bounds(_p1) || !_bmp.is_in_bounds(_p2))
        {
            Print::ln("WARN: tried drawing point outseide bitmap bounds.");
            return;
        }

        Line line {{_p1.x, _p1.y}, {_p2.x, _p2.y}};

        uint y_prev = _p1.y;

        for(uint x = _p1.x; x < _p2.x; x++)
        {
            uint y = (uint) line[(double) x];

            _bmp[x, y] = _px;

            // keep track of changes in y larger than one pixel
            uint y_change = abs(((int)y_prev - (int)y)); 

            // Fill in gaps from large k-values
            if(y_change > 1)
            {
                for(uint i = 0; i < abs((int)y_change); i++)
                {
                    if(line.k > 0)
                        _bmp[x, y-i] = _px;
                    else
                        _bmp[x, y+i] = _px;
                }
            }

            y_prev = y;
        }
            
    }

    /** Draw pixelated line using kx + m where pixels are drawn using step sizes related to the k-value to generate the intermediate pixels when the slope is large. */
    void line_kxm_2(Bitmap& _bmp, u2 _p1, u2 _p2)
    {

        if(!_bmp.is_in_bounds(_p1) || !_bmp.is_in_bounds(_p2))
        {
            Print::ln("WARN: tried drawing point outseide bitmap bounds.");
            return;
        }

        // Vertical line
        if(_p1.x == _p2.x)
        {
            uint x = _p1.x;
            uint y_min = _p1.y > _p2.y ? _p2.y : _p1.y;
            uint y_max = _p1.y > _p2.y ? _p1.y : _p2.y;

            for(uint y = y_min; y < y_max; y++)
                _bmp[x, y] = pixel;
        }

        // Horizontal line
        if(_p1.y == _p2.y)
        {
            uint y = _p1.y;
            uint x_min = _p1.x > _p2.x ? _p2.x : _p1.x;
            uint x_max = _p1.x > _p2.x ? _p1.x : _p2.x;

            for(uint x = x_min; x < x_max; x++)
                _bmp[x, y] = pixel;
        }

        // enforce p1 to have the lower x-value
        // This was done to keep the algorithm intact, which relies on left to right drawing
        if(_p1.x > _p2.x)
        {
            u2 p_tmp = _p1;
            _p1 = _p2;
            _p2 = p_tmp;
        }

        Line line {{_p1.x, _p1.y}, {_p2.x, _p2.y}};

        double step_size = fabs(1/(line.k));

        for(double x = line.p1.x; x < line.p2.x; x = x + step_size)
        {
            double y = line[x];
            _bmp[(uint)x, (uint)y] = pixel;
        }
    }


    void triangle_no_fill(Bitmap& _bitmap, u2 _p1, u2 _p2, u2 _p3)
    {
        line_kxm_2(_bitmap, _p1, _p2);
        line_kxm_2(_bitmap, _p2, _p3);
        line_kxm_2(_bitmap, _p3, _p1);
    }

    void triangle(Bitmap& _bitmap, u2 _p1, u2 _p2, u2 _p3)
    {
        // uint x_min_23 = Min(_p2.x, _p3.x);
        uint x_min = Min(_p1.x, Min(_p2.x, _p3.x));
        uint x_max = Max(_p1.x, Max(_p2.x, _p3.x));
        uint y_min = Min(_p1.y, Min(_p2.y, _p3.y));
        uint y_max = Max(_p1.y, Max(_p2.y, _p3.y));

        for(uint x = x_min; x < x_max; x++)
        {
            for(uint y = y_min; y < y_max; y++)
            {
                // _bitmap[x, y] = pixel;
            }
        }
    }
};




#include <wayland-client.h>
#include "swrend/wayland.hh"

static void registry_global(
    void *data,
    struct wl_registry *registry,
    uint32_t name,
    const char *interface,
    uint32_t version)
{
    printf("global: %s, version: %u, name: %u\n",
           interface, version, name);
}

static void registry_global_remove(
    void *data,
    struct wl_registry *registry,
    uint32_t name)
{
    printf("global removed: %u\n", name);
}

static const struct wl_registry_listener listener = {
    .global        = registry_global,
    .global_remove = registry_global_remove,
};

#include "wayland_keyboard.hh"

void wayland_stuff()
{
    // opaque object passed to qayland server during communication
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        Print::ln("Failed to connect to Wayland display\n");
        return;
    }
    Print::ln("wl_display_connect: OK");

    // think: wl_display.get_registry(); !!
    struct wl_registry *registry = wl_display_get_registry(display);
    if (!registry)
    {
        Print::ln("Failed to get Wayland registry.\n");
        return;
    }
    Print::ln("wl_display_get_registry: OK");

    // Event listener
    // wl_registry_listener: an object containging a 'global' and a 'global_remove' callback functions
    wl_registry_add_listener(registry, &listener, NULL);
    Print::ln("wl_registry_add_listener: OK");

    // Process al pending requests (and will block until completed)
    int ret = wl_display_roundtrip(display) < 0;
    if (ret) {
        // communication/protocol failure
        Print::ln("communication/protocol failure");
        return;
    }
    Print::ln("wl_display_roundtrip: OK");


    printf("roundtrip returned value: %d\n", ret);
    if (ret < 0) {
        printf("display error: %d\n", wl_display_get_error(display));
    }


    printf("display fd = %d\n", wl_display_get_fd(display));
    printf("error = %d\n", wl_display_get_error(display));


    int loop_count = 0;
    // wl_display_dispatch will not return until a plroprly registered global event has been registered. I think..
    // while (wl_display_dispatch(display) != -1)
    // {
    //     printf("Loop!\n");
    //     if(loop_count++ > 100)
    //         break;
    //     // Main loop ??
    // }

    Print::buf("\n");

    Str xdg_rt_dir = XDG::get_runtime_dir();
    Print::buf("xdg_rt_dir = ");
    Print::ln(xdg_rt_dir);

    Str xdg_wayland_socket = XDG::get_wayland_display_socket_name();
    Print::buf("xdg_wayland_socket = ");
    Print::ln(xdg_wayland_socket);

    Str wl_socket_path = xdg_rt_dir + "/" + xdg_wayland_socket;
    Print::buf("wl_socket_path = ");
    Print::ln(wl_socket_path);

    Str wl_socket_read = Socket::read_from_socket(wl_socket_path);
    Print::buf("wl_socket_read = ");
    Print::ln(wl_socket_read);

}

int main(int argc, const char** argv)
{
    Print::ln("Hello from main_swrend.cc");


    // wayland_stuff();

    // test_bitmap_2x2();

    Bitmap white_2x2 {2, 2};
    white_2x2.clear({200, 200, 200});

    // Bitmap bmp {30, 20};
    // Bitmap bmp {4, 4};
    Bitmap bmp {60, 40};
    

    bmp.clear(25);

    bmp[1, 1] = {255, 255, 255};
    bmp[29, 19] = {255, 255, 255};

    // bmp[30, 19] = {255, 255, 255}; // out of bounds. Will alter the first pixel per out of bounds access return

    BitmapDrawer drawer;

    drawer.point(bmp, {7, 2}, {100, 100, 100});
    // drawer.point(bmp, {16, 8}, {200, 200, 200});

    // drawer.line_kxm_1(bmp, {2, 3}, {15, 19}, {100, 100, 100});
    // drawer.line_kxm_1(bmp, {12, 2}, {16, 18}, {100, 100, 100});
    // drawer.line_kxm_1(bmp, {20, 18}, {25, 2}, {100, 100, 100});

    // TODO: 3 bugs: swpping point order, x1=x2, y1=y2
    drawer.set_pixel_color({100, 100, 150});

    drawer.line_kxm_2(bmp, {2, 3}, {15, 19});
    drawer.line_kxm_2(bmp, {12, 2}, {16, 18}); // positive k
    drawer.line_kxm_2(bmp, {20, 18}, {25, 2}); // negative k
    // drawer.line_kxm_2(bmp, {25, 2}, {20, 18}); // x1 > x2
    // drawer.line_kxm_2(bmp, {15, 1}, {15, 5}); // vert
    drawer.line_kxm_2(bmp, {20, 1}, {30, 1}); // hori

    // drawer.triangle_no_fill(bmp, {1, 10}, {5, 15}, {4, 19});
    drawer.triangle_no_fill(bmp, {1, 15}, {15, 30}, {9, 35});

    // drawer.triangle(bmp, {1, 10}, {5, 15}, {4, 19});

    bmp.copy(white_2x2, {40, 25});
    
    BPMIO bmp_io {bmp};

    bmp_io.Export("tmp/spots.bmp");

    Print::ln("Bye, from main_swrend.cc. \n");
    return 0;
}