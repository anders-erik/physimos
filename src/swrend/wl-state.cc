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
static void
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

static int
create_shm_file(void)
{
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        randname(name + sizeof(name) - 7);
        Print::ln(name);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

static int
allocate_shm_file(size_t size)
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




// this callback is called every time the compositor is done using the buffer
// Maybe do not destroy the buffer every render if I want the buffer to be reused! (or maybe just rebind it every time..)
static void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    // Print::ln("wl_buffer_destroy!");
    /* Sent by the compositor when it's no longer using this buffer */
    // wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};


void WLFB::init()
{
    const int width =  w;
    const int height = h;

    int size = width * height * 4;
    int stride = width * 4;

    shm_fd = allocate_shm_file(size);
    if (shm_fd == -1)
    {
        Print::ln("Failed to allocate shm file in 'init_fb'");
        return;
    }

    // we unmap data before a reallocation
    // if(state.fb.data != 0)
    //     munmap(state.fb.data, size);

    data = (uint32_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED)
    {
        close(shm_fd);
        return;
    }

    // Note: compositor allocates its own memory pool based on the fd. AFter the compoistor-allocation the currently mmaped file is no longer of any use?
    // struct wl_shm_pool *pool = wl_shm_create_pool(state.wl_shm, state.fb.shm_fd, size);
    pool = wl_shm_create_pool(wl_shm, shm_fd, size);
    // if(state.fb.buffer != 0)
    //     wl_buffer_destroy(state.fb.buffer);
    buffer = wl_shm_pool_create_buffer(pool, 0, w, h, stride, WL_SHM_FORMAT_XRGB8888);
    // wl_shm_pool_destroy(pool);
    // close(fd);


    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);

    allocated = true;
}



void WLFB::resize(i2 dims)
{
    w = dims.x;
    h = dims.y;

    int size = w * h * 4;
    int stride = w * 4;

    if(allocated)
    {
        // munmap(data, size);
        // wl_buffer_destroy(buffer);
        // wl_shm_pool_destroy(pool);
        // close(shm_fd);
        // allocated = false;
    }

    shm_fd = allocate_shm_file(size);
    if (shm_fd == -1)
    {
        Print::ln("Failed to allocate shm file in 'init_fb'");
        return;
    }

    // we unmap data before a reallocation
    // if(state.fb.data != 0)
    //     munmap(state.fb.data, size);

    data = (uint32_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED)
    {
        close(shm_fd);
        return;
    }

    // Note: compositor allocates its own memory pool based on the fd. AFter the compoistor-allocation the currently mmaped file is no longer of any use?
    // struct wl_shm_pool *pool = wl_shm_create_pool(state.wl_shm, state.fb.shm_fd, size);
    pool = wl_shm_create_pool(wl_shm, shm_fd, size);
    // if(state.fb.buffer != 0)
    //     wl_buffer_destroy(state.fb.buffer);
    buffer = wl_shm_pool_create_buffer(pool, 0, w, h, stride, WL_SHM_FORMAT_XRGB8888);
    // wl_shm_pool_destroy(pool);
    // close(fd);


    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);

    allocated = true;
}