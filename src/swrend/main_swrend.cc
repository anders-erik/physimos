
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



    // BMP IO
    
    BPMIO bmp_io {bmp};

    bmp_io.Export("tmp/spots.bmp");

    Bitmap imported_bmp = bmp_io.Import("tmp/spots.bmp");

    bmp_io.set_bitmap(imported_bmp);
    bmp_io.Export("tmp/spots_export.bmp");


    BPMIO bmp_io_2 {imported_bmp};
    bmp_io.Export("tmp/spots_export_2.bmp");


    BPMIO::SExport("tmp/static_export.bmp", bmp);
    Bitmap simport_bmp = BPMIO::SImport("tmp/static_export.bmp");
    BPMIO::SExport("tmp/static_export_1.bmp", simport_bmp);


    Bitmap font_tall = BPMIO::SImport("resources/ui/font/characters-2-tall.bmp");
    BPMIO::SExport("tmp/font_tall.bmp", font_tall);



    Print::ln("Bye, from main_swrend.cc. \n");
    return 0;
}