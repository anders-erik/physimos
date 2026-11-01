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

#include "wl-state.hh"

#include "lib/print.hh"




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





void WL_Surfaces::init(i2 _content_dims, WL_State* state)
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
    state->egl.init(state->core.display, egl.surface, egl.size);
}


void WL_Surfaces::set_content_dims(i2 _content_dims)
{
    content_dims = _content_dims;

    window.size = {     content_dims.x + window_border_width * 2, 
                        content_dims.y + window_topbar_height + window_border_width};
    
    content.size = {content_dims.x, content_dims.y};
    content.pos = {window_border_width, window_topbar_height};

    test.size = {50, 50};
    test.pos = {400, 200};

    egl.size = {100, 30};
    egl.pos = {400, 100};
}


void WL_Surfaces::new_wl_buffers(WL_State* state)
{
    wl_buffer_destroy(window.buffer);
    wl_buffer_destroy(content.buffer);
    wl_buffer_destroy(test.buffer);


    window.create_new_buffer(pool.pool, get_offset(WL_Surface::Window), state);
    content.create_new_buffer(pool.pool, get_offset(WL_Surface::Content), state);
    test.create_new_buffer(pool.pool, get_offset(WL_Surface::Test), state);

    // window.commit();
    // content.commit();
    // test.commit();

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



