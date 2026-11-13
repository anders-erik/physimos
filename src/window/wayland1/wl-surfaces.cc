#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <wayland-client.h>

#include "lib/print.hh"
#include "rend/sw/swrend.hh"

#include "wl-surfaces.hh"
#include "wayland.hh"


void WL_Decoration::draw()
{
    SWR::Renderer renderer {surface.pixel_buffer};

    PX32 edge_color = 0x00000000;
    PX32 edge_line_color = 0x66666666;

    renderer.clear(edge_color);
    // bottom
    renderer.draw_line_float_lerp({border_width-1, border_width-1}, {surface.pixel_buffer.w - border_width, border_width-1}, edge_line_color);
    // Top
    renderer.draw_line_float_lerp({border_width-1, surface.pixel_buffer.h-border_width}, {surface.pixel_buffer.w - border_width, surface.pixel_buffer.h-border_width}, edge_line_color);
    // left
    renderer.draw_line_float_lerp({border_width-1, border_width}, {border_width-1, surface.pixel_buffer.h-border_width}, edge_line_color);
    // right
    renderer.draw_line_float_lerp({surface.pixel_buffer.w - border_width, border_width}, {surface.pixel_buffer.w - border_width, surface.pixel_buffer.h-border_width}, edge_line_color);

    // Draw top bar
    PX32 topbar_color = 0xFF777777;
    renderer.draw_rectangle(    {border_width, surface.pixel_buffer.h - border_width - topbar_height}, 
                                {surface.pixel_buffer.w  - border_width, surface.pixel_buffer.h  - border_width}, 
                                topbar_color                                                                        );

    // close button
    renderer.draw_rectangle(    {surface.pixel_buffer.w - border_width - 30, surface.pixel_buffer.h - border_width - 30}, 
                                {surface.pixel_buffer.w  - border_width, surface.pixel_buffer.h  - border_width}, 
                                0xFFAA4444                                                                                     );
    // Maximize/Restore button
    renderer.draw_rectangle(    {surface.pixel_buffer.w - border_width - 60, surface.pixel_buffer.h - border_width - 30}, 
                                {surface.pixel_buffer.w - border_width - 30, surface.pixel_buffer.h  - border_width}, 
                                0xFFAAAA44                                                                                     );
    // Minimize button
    renderer.draw_rectangle(    {surface.pixel_buffer.w - border_width - 90,    surface.pixel_buffer.h - border_width - 30}, 
                                {surface.pixel_buffer.w - border_width - 60,    surface.pixel_buffer.h  - border_width}, 
                                0xFF44AA44                                                                                     );
    // renderer.draw_rectangle({0, 0}, {30, 30}, 0xFFAA4444);
}




int WL_Surface::get_size_pixels() { return pixel_buffer.w * pixel_buffer.h; };
int WL_Surface::get_size_bytes() { return pixel_buffer.w * pixel_buffer.h * 4; };



void WL_Surface::init(struct wl_compositor* compositor, struct wl_shm_pool* pool, int offset, Wayland* state)
{
    surface = wl_compositor_create_surface(compositor);

    buffer = wl_shm_pool_create_buffer(pool, offset, pixel_buffer.w, pixel_buffer.h, pixel_buffer.w*4, format);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, state); 
    // wl_surface_attach(surface, buffer, 0, 0);
}

void WL_Surface::create_new_buffer(struct wl_shm_pool* pool, int offset, Wayland* state)
{
    buffer = wl_shm_pool_create_buffer(pool, offset, pixel_buffer.w, pixel_buffer.h, pixel_buffer.w*4, format);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, state);
    // wl_surface_attach(surface, buffer, 0, 0);
}

void WL_Surface::create_subsurface(struct wl_subcompositor* subcompositor, struct wl_surface* parent)
{
    subsurface = wl_subcompositor_get_subsurface(subcompositor, surface, parent);

    if (!subsurface)
    {
        fprintf(stderr, "Failed to create subsurface.\n");
        return;
    }

    wl_subsurface_set_position(subsurface, pos.x, pos.y);
}

void WL_Surface::clear(PX32 color)
{
    for(uint i = 0; i < get_size_bytes(); i++)
    {
        (pixel_buffer.buf)[i] = color;
    }
}

void WL_Surface::commit()
{
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage_buffer(surface, 0, 0, pixel_buffer.w, pixel_buffer.h);
    wl_surface_commit(surface);
    surface_is_busy = true;
}





/* Shared memory support code */
void WL_Pool::
randname(char *buf)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i) {
        buf[i] = 'A'+(r&15)+(r&16)*2;
        r >>= 5;
    }
}

int WL_Pool::
create_shm_file(void)
{
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        randname(name + sizeof(name) - 7);
        // Print::ln(name);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

int WL_Pool::
allocate_shm_file(int size)
{
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) {
        close(fd);
        return -1;
    }
    return fd;
}


void WL_Pool::init(int _size)
{
    if(data != nullptr)
    {
        Print::ln("WARN: tried to initialize an already allocated WL_Pool in 'WL_Pool::init';");
        return;
    }

    size = _size;

    shm_fd = allocate_shm_file(size);
    if (shm_fd == -1)
    {
        Print::ln("Failed to allocate shm file in 'WL_Pool::init'");
        return;
    }

    data = (uint32_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED)
    {
        Print::ln("ERROR: mmap failed in WL_Pool::init.");
        close(shm_fd);
        return;
    }

    pool = wl_shm_create_pool(wl_shm, shm_fd, size);


    // Note: compositor allocates its own memory pool based on the fd. AFter the compoistor-allocation the currently mmaped file is no longer of any use?
    close(shm_fd);
}





void WL_Surfaces::init(i2 _content_dims, Wayland* state)
{   
    i2 window_dims = content_to_window_dims(_content_dims);
    set_surface_dims(window_dims);

    pool.init(get_all_surface_buffer_sizes_in_bytes() * 3);

    decorations.surface.pixel_buffer.buf = (PX32*) get_buffer_pointer(WL_Surface::Window);
    content.pixel_buffer.buf = (PX32*) get_buffer_pointer(WL_Surface::Content);
    test.pixel_buffer.buf = (PX32*) get_buffer_pointer(WL_Surface::Test);

    SWR::Renderer renderer;
    decorations.draw();
    renderer.set_buffer(content.pixel_buffer);
    renderer.clear(0xFF000000);
    renderer.set_buffer(test.pixel_buffer);
    renderer.clear(0xFF000000);

    decorations.surface.init(state->core.compositor, pool.pool, get_offset(WL_Surface::Window), state);

    content.init(state->core.compositor, pool.pool, get_offset(WL_Surface::Content), state);
    test.init(state->core.compositor, pool.pool, get_offset(WL_Surface::Test), state);
    
    content.create_subsurface(state->core.subcompositor, decorations.surface.surface);
    test.create_subsurface(state->core.subcompositor, decorations.surface.surface);

    egl.surface = wl_compositor_create_surface(state->core.compositor);
    egl.create_subsurface(state->core.subcompositor, decorations.surface.surface);
    // state->egl.init(state->core.display, egl.surface, egl.pixel_buffer.size());
    state->egl.init(state->core.display, egl.surface, {100, 100});
}


void WL_Surfaces::full_reload_from_window_size(i2 new_window_dims, Wayland* wayland)
{
    window_size_change_detected = false;

    decorations.surface.pixel_buffer = PixelBuffer{nullptr, new_window_dims.x, new_window_dims.y, PX32F::ARGB, PixelBuffer::Top};


    i2 new_content_dims = window_to_content_dims(size_change_dims);
    content.pixel_buffer = PixelBuffer{nullptr, new_content_dims.x, new_content_dims.y, PX32F::ARGB, PixelBuffer::Top};
    content.pos = {decorations.border_width, decorations.topbar_height + decorations.border_width};

    test.pixel_buffer = PixelBuffer{nullptr, 50, 50, PX32F::ARGB, PixelBuffer::Top};
    test.pos = {400, 200};

    egl.pixel_buffer = PixelBuffer{nullptr, 50, 50, PX32F::ARGB, PixelBuffer::Top};
    egl.pos = {400, 100};



    decorations.surface.pixel_buffer.buf = (PX32*) get_buffer_pointer(WL_Surface::Window);
    content.pixel_buffer.buf = (PX32*) get_buffer_pointer(WL_Surface::Content);
    test.pixel_buffer.buf = (PX32*) get_buffer_pointer(WL_Surface::Test);

    SWR::Renderer renderer;
    decorations.draw();
    renderer.set_buffer(content.pixel_buffer);
    renderer.clear(0xFF000000);
    renderer.set_buffer(test.pixel_buffer);
    renderer.clear(0xFF000000);


    // WAYLAND COMMUNIACTION

    wl_buffer_destroy(decorations.surface.buffer);
    wl_buffer_destroy(content.buffer);
    wl_buffer_destroy(test.buffer);

    decorations.surface.create_new_buffer(pool.pool, get_offset(WL_Surface::Window), wayland);
    content.create_new_buffer(pool.pool, get_offset(WL_Surface::Content), wayland);
    test.create_new_buffer(pool.pool, get_offset(WL_Surface::Test), wayland);

    decorations.surface.commit();
    content.commit();
    test.commit();
}





i2  WL_Surfaces::content_to_window_dims(i2 content_dims)
{
    return  {   content_dims.x + decorations.border_width * 2, 
                content_dims.y + decorations.topbar_height + decorations.border_width * 2 };
}
i2  WL_Surfaces::window_to_content_dims(i2 window_dims)
{
    return  {   window_dims.x - decorations.border_width * 2, 
                window_dims.y - decorations.topbar_height - decorations.border_width * 2 };
}


int WL_Surfaces::get_all_surface_buffer_sizes_in_bytes()
{
    return (decorations.surface.get_size_bytes() + content.get_size_bytes() + test.get_size_bytes() + egl.get_size_bytes());
}

int WL_Surfaces::get_offset(WL_Surface::Type _surface_type)
{
    switch (_surface_type)
    {
    case WL_Surface::Window:
        return 0;
        break;
    case WL_Surface::Content:
        return get_offset(WL_Surface::Window) + decorations.surface.get_size_bytes();
        break;
    case WL_Surface::Test:
        return get_offset(WL_Surface::Content) + content.get_size_bytes();
        break;
    
    default:
        break;
    }

    return 0;
}


void* WL_Surfaces::get_buffer_pointer(WL_Surface::Type surface_type)
{
    return ((uint8_t*)pool.data) + get_offset(surface_type);
}

void WL_Surfaces::set_surface_dims(i2 window_dims)
{
    decorations.surface.pixel_buffer = PixelBuffer{nullptr, window_dims.x, window_dims.y, PX32F::RGBA, PixelBuffer::Top};
    decorations.surface.format = wl_shm_format::WL_SHM_FORMAT_ARGB8888;;

    i2 content_dims = window_to_content_dims(window_dims);
    content.pixel_buffer = PixelBuffer{nullptr, content_dims.x, content_dims.y, PX32F::ARGB, PixelBuffer::Top};
    content.pos = {decorations.border_width, decorations.topbar_height + decorations.border_width};

    test.pixel_buffer = PixelBuffer{nullptr, 50, 50, PX32F::ARGB, PixelBuffer::Top};
    test.pos = {400, 200};

    egl.pixel_buffer = PixelBuffer{nullptr, 50, 50, PX32F::ARGB, PixelBuffer::Top};
    egl.pos = {400, 100};

}



void WL_Surfaces::commit()
{
    decorations.surface.commit();
    content.commit();
    test.commit();
    // egl.commit();
}


