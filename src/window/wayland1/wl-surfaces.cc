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

#include "wl-surfaces.hh"
#include "wayland.hh"

#include "lib/print.hh"






int WL_Surface::get_size_pixels() { return pixel_buffer.w * pixel_buffer.h; };
int WL_Surface::get_size_bytes() { return pixel_buffer.w * pixel_buffer.h * 4; };



void WL_Surface::init(struct wl_compositor* compositor, struct wl_shm_pool* pool, int offset, Wayland* state)
{
    surface = wl_compositor_create_surface(compositor);

    buffer = wl_shm_pool_create_buffer(pool, offset, pixel_buffer.w, pixel_buffer.h, pixel_buffer.w*4, wl_shm_format::WL_SHM_FORMAT_XRGB8888);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, state); 
    // wl_surface_attach(surface, buffer, 0, 0);
}

void WL_Surface::create_new_buffer(struct wl_shm_pool* pool, int offset, Wayland* state)
{
    buffer = wl_shm_pool_create_buffer(pool, offset, pixel_buffer.w, pixel_buffer.h, pixel_buffer.w*4, wl_shm_format::WL_SHM_FORMAT_XRGB8888);
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
    set_content_dims(_content_dims);

    pool.init(get_all_surface_buffer_sizes_in_bytes() * 3);


    window.init(state->core.compositor, pool.pool, get_offset(WL_Surface::Window), state);
    content.init(state->core.compositor, pool.pool, get_offset(WL_Surface::Content), state);
    test.init(state->core.compositor, pool.pool, get_offset(WL_Surface::Test), state);
    
    content.create_subsurface(state->core.subcompositor, window.surface);
    test.create_subsurface(state->core.subcompositor, window.surface);

    egl.surface = wl_compositor_create_surface(state->core.compositor);
    egl.create_subsurface(state->core.subcompositor, window.surface);
    // state->egl.init(state->core.display, egl.surface, egl.pixel_buffer.size());
    state->egl.init(state->core.display, egl.surface, {100, 100});
}


i2  WL_Surfaces::content_to_window_dims(i2 _content_dims)
{
    return  {   content_dims.x + window_border_width * 2, 
                content_dims.y + window_topbar_height + window_border_width };
}
i2  WL_Surfaces::window_to_content_dims(i2 _window_dims)
{
    return  {   _window_dims.x - window_border_width * 2, 
                _window_dims.y - window_topbar_height - window_border_width };
}


int WL_Surfaces::get_all_surface_buffer_sizes_in_bytes()
{
    return (window.get_size_bytes() + content.get_size_bytes() + test.get_size_bytes() + egl.get_size_bytes());
}

int WL_Surfaces::get_offset(WL_Surface::Type _surface_type)
{
    switch (_surface_type)
    {
    case WL_Surface::Window:
        return 0;
        break;
    case WL_Surface::Content:
        return get_offset(WL_Surface::Window) + window.get_size_bytes();
        break;
    case WL_Surface::Test:
        return get_offset(WL_Surface::Content) + content.get_size_bytes();
        break;
    
    default:
        break;
    }

    return 0;
}

void* WL_Surfaces::get_content_pointer()
{
    return (void*) ((char*)pool.data + window.get_size_bytes());
}

void WL_Surfaces::set_content_dims(i2 _content_dims)
{
    content_dims = _content_dims;

    window.pixel_buffer.w = content_dims.x + window_border_width * 2;
    window.pixel_buffer.h = content_dims.y + window_topbar_height + window_border_width;
    
    content.pixel_buffer.w = content_dims.x;
    content.pixel_buffer.h = content_dims.y;
    content.pos = {window_border_width, window_topbar_height};

    test.pixel_buffer.w = 50;
    test.pixel_buffer.h = 50;
    test.pos = {400, 200};

    egl.pixel_buffer.w = 30;
    egl.pixel_buffer.h = 100;;
    egl.pos = {400, 100};
}


void WL_Surfaces::full_reload_from_window_size(i2 new_window_dims)
{


    window_size_change_detected = false;

    window.pixel_buffer = PixelBuffer{nullptr, new_window_dims.x, new_window_dims.y, PX32F::ARGB, PixelBuffer::Top};

    // content.size = window_to_content_dims(window.pixel_buffer.size());
    content.pos = {window_border_width, window_topbar_height};

    // test.size = {50, 50};
    test.pixel_buffer = PixelBuffer{nullptr, 50, 50, PX32F::ARGB, PixelBuffer::Top};
    test.pos = {400, 200};

    egl.pixel_buffer = PixelBuffer{nullptr, 50, 50, PX32F::ARGB, PixelBuffer::Top};
    egl.pos = {400, 100};

    i2 new_content_dims = window_to_content_dims(size_change_dims);
    set_content_dims(new_content_dims);
}


// void WL_Surfaces::reload_with_new_window_dims(i2 new_window_dims)
// {
//     window.size = new_window_dims;
    
//     content.size = window_to_content_dims(new_window_dims);
//     content.pos = {window_border_width, window_topbar_height};

//     test.size = {50, 50};
//     test.pos = {400, 200};

//     egl.size = {100, 30};
//     egl.pos = {400, 100};

//     i2 new_content_dims = window_to_content_dims(size_change_dims);
//     set_content_dims(new_content_dims);
// }


void WL_Surfaces::new_wl_buffers(Wayland* wayland)
{
    // old_buffers.append({WL_Surface::Window, window.buffer});
    // old_buffers.append({WL_Surface::Content, content.buffer});
    // old_buffers.append({WL_Surface::Test, test.buffer});

    wl_buffer_destroy(window.buffer);
    wl_buffer_destroy(content.buffer);
    wl_buffer_destroy(test.buffer);

    // old_buffers.push_back({WL_Surface::Window, window.buffer});
    // old_buffers.push_back({WL_Surface::Content, content.buffer});
    // old_buffers.push_back({WL_Surface::Test, test.buffer});



    window.create_new_buffer(pool.pool, get_offset(WL_Surface::Window), wayland);
    content.create_new_buffer(pool.pool, get_offset(WL_Surface::Content), wayland);
    test.create_new_buffer(pool.pool, get_offset(WL_Surface::Test), wayland);

    window.commit();
    content.commit();
    test.commit();

    // wl_display_dispatch(state->core.display);

    // window.surface_is_busy = false;
    // content.surface_is_busy = false;
    // test.surface_is_busy = false;



    // content.create_new_buffer(pool.pool, get_offset(WL_Surface::Content), state);
    // test.create_new_buffer(pool.pool, get_offset(WL_Surface::Test), state);
}

void WL_Surfaces::commit()
{
    window.commit();
    content.commit();
    test.commit();
    // egl.commit();
}


