
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

#include "swrend/bitmap.hh"
#include "swrend/BMP.hh"
#include "swrend/line.hh"

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> // socket
#include <sys/un.h> // sockaddr_un


#include "lib/str.hh"


#include "swrend/wayland.hh"


void test_bitmap_2x2()
{
    Bitmap bmp {2, 2};

    bmp.clear(0x010203FF);

    Str pixel_str;

    pixel_str = PX::to_str(bmp[0, 0]);
    Print::ln(pixel_str);

    bmp[0, 0] = 0xFFFFFFFF;
    pixel_str = PX::to_str(bmp[0, 0]);
    Print::ln(pixel_str);

    bmp[0, 0] = 0xFFFFFFFF;
    Print::ln(PX::to_str_hex(bmp[0, 0]));
    bmp[0, 0] = 0x004080FF;
    Print::ln(PX::to_str_hex(bmp[0, 0]));

    // BPMIO::Export("tmp/2x2.bmp", bmp);


    BMPFileInfoHeader file_info_header {bmp};
    Vec<uint8_t> header_buff = file_info_header.get_header_buff();
    // auto vec = file_info_header.get_BM();
    Print::ln(file_info_header.to_str());

    BMPDIBHeader file_DIB_header {bmp};
    Vec<uint8_t> DIB_buff = file_DIB_header.get_DIB_buffer();

    
    bmp[0, 0] = 0x222222FF;
    bmp[1, 0] = 0x555555FF;
    bmp[0, 1] = 0x999999FF;
    bmp[1, 1] = 0xBBBBBBFF;



    BMPIO bmp_io {bmp};
    bmp_io.Export("tmp/2x2.bmp");
}

// Str logf_path = "tmp/log.txt";
// void logf(Str _msg, )
// {

// }




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

    // Pixel pixel;
    PX32 pixel;

    // void set_pixel_color(Pixel _pixel)
    // {
    //     pixel = _pixel;
    // }

    void set_pixel_color(PX32 _pixel)
    {
        pixel = _pixel;
    }

    void point(Bitmap& _bmp, u2 _p, Pixel _px)
    {
        if(_bmp.is_in_bounds(_p))
            _bmp[_p] = _px.to_PX32RGBA();
        else
            Print::ln("WARN: tried drawing point outseide bitmap bounds.");
    }

    void point(Bitmap& _bmp, u2 _p, PX32 _px)
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

            _bmp[x, y] = _px.to_PX32RGBA();

            // keep track of changes in y larger than one pixel
            uint y_change = abs(((int)y_prev - (int)y)); 

            // Fill in gaps from large k-values
            if(y_change > 1)
            {
                for(uint i = 0; i < abs((int)y_change); i++)
                {
                    if(line.k > 0)
                        _bmp[x, y-i] = _px.to_PX32RGBA();
                    else
                        _bmp[x, y+i] = _px.to_PX32RGBA();
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





int main(int argc, const char** argv)
{
    Print::ln("Hello from main_swrend.cc");

    if(true)
    {
        Wayland wayland;

        if(wayland.setup_ok)
            wayland.run();
        
        return 0;
    }
    // WL_SHM_FORMAT_RGB888
    // WL_SHM_FORMAT_RGBA8888


    // test_bitmap_2x2();

    Bitmap white_2x2 {2, 2};
    // white_2x2.clear(0xAABBCCFF);
    white_2x2.clear(0xAABBCCFF);

    Bitmap white_2x2_ARGB {2, 2, PX32F::ARGB};
    white_2x2_ARGB.clear(0xFFAABBCC);
    // white_2x2_ARGB.set_format(PX32F::ARGB);


    // Bitmap bmp {30, 20};
    // Bitmap bmp {4, 4};
    Bitmap bmp {60, 40};
    

    bmp.clear(25);

    bmp[1, 1] = 0xFFFFFFFF;
    bmp[29, 19] = 0xFFFFFFFF;

    // bmp[30, 19] = {255, 255, 255}; // out of bounds. Will alter the first pixel per out of bounds access return

    BitmapDrawer drawer;

    drawer.point(bmp, {7, 2}, {100, 100, 100});
    // drawer.point(bmp, {16, 8}, {200, 200, 200});

    // drawer.line_kxm_1(bmp, {2, 3}, {15, 19}, {100, 100, 100});
    // drawer.line_kxm_1(bmp, {12, 2}, {16, 18}, {100, 100, 100});
    // drawer.line_kxm_1(bmp, {20, 18}, {25, 2}, {100, 100, 100});

    // TODO: 3 bugs: swpping point order, x1=x2, y1=y2
    // drawer.set_pixel_color({100, 100, 150});
    // drawer.set_pixel_color(0x555588FF);
    drawer.set_pixel_color(0x885555FF);
    // drawer.set_pixel_color(0xFFFFFFFF);
    // drawer.set_pixel_color(0x000000FF);

    drawer.line_kxm_2(bmp, {2, 3}, {15, 19});
    drawer.line_kxm_2(bmp, {12, 2}, {16, 18}); // positive k
    drawer.line_kxm_2(bmp, {20, 18}, {25, 2}); // negative k
    // drawer.line_kxm_2(bmp, {25, 2}, {20, 18}); // x1 > x2
    // drawer.line_kxm_2(bmp, {15, 1}, {15, 5}); // vert
    drawer.line_kxm_2(bmp, {20, 1}, {30, 1}); // hori

    // drawer.triangle_no_fill(bmp, {1, 10}, {5, 15}, {4, 19});
    drawer.triangle_no_fill(bmp, {1, 15}, {15, 30}, {9, 35});

    // drawer.triangle(bmp, {1, 10}, {5, 15}, {4, 19});

    bmp.paste(white_2x2, {40, 25});



    // BMP IO
    
    // BMPIO bmp_io {bmp};

    // bmp_io.Export("tmp/spots.bmp");

    // Bitmap imported_bmp = bmp_io.Import("tmp/spots.bmp");

    // bmp_io.set_bitmap(imported_bmp);
    // bmp_io.Export("tmp/spots_export.bmp");


    // BMPIO bmp_io_2 {imported_bmp};
    // bmp_io.Export("tmp/spots_export_2.bmp");


    BMPIO::SExport_PX32RGBA("tmp/white2x2.bmp", white_2x2);
    Bitmap simport_white2x2 = BMPIO::SImport_PX32("tmp/white2x2.bmp");
    BMPIO::SExport_PX32RGBA("tmp/white2x2_export.bmp", simport_white2x2);

    BMPIO::SExport_PX32RGBA("tmp/static_export.bmp", bmp);
    Bitmap simport_bmp = BMPIO::SImport_PX32("tmp/static_export.bmp");
    BMPIO::SExport_PX32RGBA("tmp/static_export_1.bmp", simport_bmp);


    Bitmap font_tall = BMPIO::SImport_PX32("resources/ui/font/characters-2-tall.bmp");
    BMPIO::SExport_PX32RGBA("tmp/font_tall.bmp", font_tall);

    char letter = 'a';
    uint letter_height_offset = (letter - 30) * 150;
    u2 pos = {0, letter_height_offset};
    u2 size = {80, 150};
    Bitmap bmp_a = font_tall.get_subbitmap(pos, size);
    BMPIO::SExport_PX32RGBA("tmp/a.bmp", bmp_a);

    Bitmap intersected_a = Bitmap::intersection(bmp, bmp_a, {20, 5});
    bmp.paste(intersected_a, {20, 5}); // out of bounds copy
    BMPIO::SExport_PX32RGBA("tmp/static_export_2.bmp", bmp);

    Bitmap a_scale_2 = bmp_a.scale(2.0);
    BMPIO::SExport_PX32RGBA("tmp/a_scale_2.bmp", a_scale_2);

    Bitmap a_scale_05 = bmp_a.scale(0.5);
    BMPIO::SExport_PX32RGBA("tmp/a_scale_05.bmp", a_scale_05);

    Bitmap a_scale_01 = bmp_a.scale(0.1);
    BMPIO::SExport_PX32RGBA("tmp/a_scale_01.bmp", a_scale_01);

    Bitmap a_scale_02 = bmp_a.scale(0.2);
    BMPIO::SExport_PX32RGBA("tmp/a_scale_02.bmp", a_scale_02);

    bmp.paste(a_scale_02, {20, 5}); // out of bounds copy
    BMPIO::SExport_PX32RGBA("tmp/static_export_3.bmp", bmp);



    Print::ln("Bye, from main_swrend.cc. \n");
    return 0;
}