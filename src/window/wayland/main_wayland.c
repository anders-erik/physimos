#define _POSIX_C_SOURCE 200112L
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
// #include <cstdlib>
// #include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <linux/input-event-codes.h>

#include <wayland-client.h>
#include "protocols/xdg-shell-client-protocol.h"

#include "input.hh"

// #include "lib/print.hh"
// #include "math/vecmat.h"

static struct wl_display* display;
static struct wl_registry* registry;

// Below triplet is checked in main to validate wayland globals
// These are all wayland objects
static struct wl_compositor* compositor;
static struct xdg_wm_base* wm_base;     // Generated using wayland-scanner
static struct wl_shm* shm;              // part of global handler and pool creation

// struct wl_shm_pool* pool;
// static struct wl_buffer* buffer;
// static uint32_t* buf_map;

static struct wl_surface* surface;
static struct xdg_surface* xdg_surface; // Generated using wayland-scanner
static struct xdg_toplevel* toplevel;   // Generated using wayland-scanner
static struct wl_seat* wl_seat;
static struct wl_pointer* pointer;
static struct wl_keyboard* keyboard;

// event loop variables
static const struct wl_callback_listener wl_surface_frame_listener;
float offset;

// int fd;
// float w_width = 200.0f;
// float w_height = 100.0f;

typedef struct WFB {
    int width;
    int height;
    int pixel_size; // 4 bytes for XRGB8888
    int stride;
    int size;
    uint32_t* data;
    int fd;
    struct wl_shm_pool* pool;
    struct wl_buffer* buf;
} WFB;

#define FB_INIT_WIDTH       200
#define FB_INIT_HEIGHT      100
#define FB_INIT_PIXEL_SIZE  4 // XRGB8888
#define FB_INIT_STRIDE      (FB_INIT_WIDTH * FB_INIT_PIXEL_SIZE)
#define FB_INIT_SIZE        (FB_INIT_STRIDE * FB_INIT_HEIGHT)

static WFB fb = {
    .width = FB_INIT_WIDTH,
    .height = FB_INIT_HEIGHT,
    .pixel_size = FB_INIT_PIXEL_SIZE,
    .stride = FB_INIT_STRIDE,
    .size = FB_INIT_SIZE,
    .data = NULL,
    .fd = -1,
    .pool = NULL,
    .buf = NULL
};

typedef struct WFrame {
    uint32_t time_prev; // time of the previous frame
    uint32_t count;     // number of frames rendered
} WFrame;

typedef struct WInput_ {
    int close_flag;
    struct wl_seat* wl_seat;
    struct wl_pointer* pointer;
    struct wl_keyboard* keyboard;
} WInput_;

typedef struct WWindow_ {
    struct wl_surface* surface;
    struct xdg_surface* xdg_surface;
    struct xdg_toplevel* toplevel;  
} WWindow_;

typedef struct WGlobal_ {
    struct wl_compositor* compositor;
    struct xdg_wm_base* wm_base;
    struct wl_shm* shm;
    struct wl_registry* registry;
    struct wl_display* display;
} WGlobal_;

typedef struct Wstate {
    WGlobal_ global;
    WWindow_ window;
    WInput_  input;
    WFB     fb;
    WFrame  frame;
} Wstate;

static Wstate state;



static void shm_format(void* data, struct wl_shm* shm, uint32_t format)
{
    fprintf(stderr, "Received shm format: %u\n", format);
    if (format == WL_SHM_FORMAT_XRGB8888) {
        fprintf(stderr, "Supported format: WL_SHM_FORMAT_XRGB8888\n");
    } else {
        fprintf(stderr, "Unsupported format: %u\n", format);
    }
}

static const struct wl_shm_listener shm_listener = {
    .format = shm_format
};



/** Callback for each available global object after adding linstener to registry */
static void global_handler(void* data, struct wl_registry* reg, uint32_t id,
                           const char* interface, uint32_t version)
{
    fprintf(stderr, "Global object added: %u, interface: %s, version: %u\n", id, interface, version);

    if (strcmp(interface, wl_compositor_interface.name) == 0)
        compositor = wl_registry_bind(reg, id, &wl_compositor_interface, 4);
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        wm_base = wl_registry_bind(reg, id, &xdg_wm_base_interface, 1);
    else if (strcmp(interface, wl_shm_interface.name) == 0) {
        shm = wl_registry_bind(reg, id, &wl_shm_interface, 1);
        wl_shm_add_listener(shm, &shm_listener, NULL);
    }
    else if(strcmp(interface, wl_seat_interface.name) == 0)
    {
        wl_seat = wl_registry_bind(registry, id, &wl_seat_interface, 1);

        pointer = wl_seat_get_pointer(wl_seat);
        wl_pointer_add_listener(pointer, &pointer_listener, NULL);

        keyboard = wl_seat_get_keyboard(wl_seat);
        wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
    }
    // wl_seat_interface
}

static void global_remove(void* data, struct wl_registry* reg, uint32_t id)
{
    fprintf(stderr, "Global object removed: %u\n", id);
}

static const struct wl_registry_listener registry_listener = {
    .global = global_handler,
    .global_remove = global_remove
};

static void xdg_ping(void* data, struct xdg_wm_base* wm, uint32_t serial) {
    xdg_wm_base_pong(wm, serial);
    // fprintf(stderr, "Received ping from compositor with serial: %u\n", serial);
}

static const struct xdg_wm_base_listener wm_base_listener = {
    .ping = xdg_ping
};

static void xdg_surface_configure(void* data, struct xdg_surface* s, uint32_t serial) {
    xdg_surface_ack_configure(s, serial);
    wl_surface_attach(surface, fb.buf, 0, 0);
    wl_surface_commit(surface);
    fprintf(stdout, "xdg_surface_configure:: serial: %u\n", serial);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure
};

void buffer_release_handler(void *data, struct wl_buffer *buffer)
{
    // printf("Buffer released!\n");
}
static const struct wl_buffer_listener buffer_listener = {
    .release = buffer_release_handler
};

int fb_init(WFB* fb)
{
    char filename[] = "/tmp/wayland-shm-XXXXXX";
    fb->fd = mkstemp(filename);
    unlink(filename); // unlinked immediately

    ftruncate(fb->fd, fb->size);
    fb->data = mmap(NULL, fb->size, PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);
    fb->pool = wl_shm_create_pool(shm, fb->fd, fb->size);

    return 0;
}

static int fb_update_buffer(WFB* fb_)
{
    // set contents
    memset(fb_->data, state.frame.count, fb_->size);

    // pool = wl_shm_create_pool(shm, fb_->fd, fb_->size);
    fb_->buf = wl_shm_pool_create_buffer(   fb_->pool, 
                                            0, 
                                            fb_->width, 
                                            fb_->height, 
                                            fb_->stride, 
                                            WL_SHM_FORMAT_XRGB8888      );
    // wl_shm_pool_destroy(fb_->pool);
    return 0;
}


void fb_update_size(WFB* fb_, int width, int height)
{
    wl_shm_pool_destroy(fb_->pool);
    munmap(fb_->data, fb_->size);

    fb_->width = width;
    fb_->height = height;
    fb_->stride = width * fb_->pixel_size; // assuming 4 bytes per pixel
    fb_->size = fb_->stride * height;

    ftruncate(fb_->fd, fb_->size);
    fb_->data = mmap(NULL, fb_->size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_->fd, 0);
    fb_->pool = wl_shm_create_pool(shm, fb_->fd, fb_->size);
}



// Creates a shm buffer of given size and returns a wl_buffer
// static struct wl_buffer* create_buffer(FB* fb_)
// {
//     ftruncate(fb_->fd, fb_->size);
//     fb_->data = mmap(NULL, fb_->size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_->fd, 0);

//     fb.buf = fb_update_buffer(fb_);

//     pool = wl_shm_create_pool(shm, fb_->fd, fb_->size);
//     struct wl_buffer* buf_tmp = wl_shm_pool_create_buffer(fb_->pool, 0,
//         fb_->width, fb_->height, fb_->stride, WL_SHM_FORMAT_XRGB8888);
    
//     wl_shm_pool_destroy(pool);
//     munmap(fb_->data, fb_->size);


//     return buf_tmp;
// }



static void
wl_surface_frame_done(void *data, struct wl_callback *cb, uint32_t time)
{
    state.frame.count++;
    // fprintf(stderr, "Frame cssallback:  time: %u, count: %i\n", time, frame_count);

    // Keep listening for the next frame?
    wl_callback_destroy(cb);
    cb = wl_surface_frame(surface);
	wl_callback_add_listener(cb, &wl_surface_frame_listener, NULL);


    // Update the framebuffer
    fb_update_buffer(&fb);
    wl_buffer_add_listener(fb.buf, &buffer_listener, NULL);

    // reload surface with updated buffer
	wl_surface_attach(surface, fb.buf, 0, 0);
	wl_surface_damage_buffer(surface, 0, 0, INT32_MAX, INT32_MAX);
	wl_surface_commit(surface);

    
    state.frame.time_prev = time;
}

static const struct wl_callback_listener wl_surface_frame_listener = {
    .done = wl_surface_frame_done,
};


int wayland_globals_init()
{
    display = wl_display_connect(NULL); // core
    if (!display) {
        fprintf(stderr, "No Wayland display.\n");
        return 1;
    }
    fprintf(stderr, "Wayland display connected.\n");

    registry = wl_display_get_registry(display); // protocol
    wl_registry_add_listener(registry, &registry_listener, NULL); // protocol
    wl_display_roundtrip(display); // core

    // If global singletons were not found in registry
    if (!compositor || !wm_base || !shm) {
        fprintf(stderr, "Missing required Wayland globals.\n");
        return 1;
    }

    return 0;
}

int wayland_surface_init()
{
    fb_init(&fb);
    fb_update_size(&fb, 200, 100);
    fb_update_buffer(&fb);
    // buffer = create_buffer(&fb);


    xdg_wm_base_add_listener(wm_base, &wm_base_listener, NULL);

    surface = wl_compositor_create_surface(compositor);
    xdg_surface = xdg_wm_base_get_xdg_surface(wm_base, surface);
    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, NULL);
    toplevel = xdg_surface_get_toplevel(xdg_surface);

    xdg_toplevel_set_title(toplevel, "Wayland shm window");

    wl_surface_attach(surface, fb.buf, 0, 0);
	wl_surface_damage_buffer(surface, 0, 0, INT32_MAX, INT32_MAX);
    wl_surface_commit(surface);

    // frame callback
    struct wl_callback *cb = wl_surface_frame(surface);
    wl_callback_add_listener(cb, &wl_surface_frame_listener, NULL);
}

int wayland_close(Wstate* state)
{

    return 0;
}

int wayland_cleanup()
{
    close(fb.fd);
    wl_display_disconnect(display);
    fprintf(stderr, "Wayland display disconnected.\n");
    return 0;
}

int main()
{
    int res = 0;
    res = wayland_globals_init();
    if(res != 0) {
        fprintf(stderr, "Failed to initialize Wayland globals.\n");
        return -1;
    }

    res = wayland_surface_init();
    if(res != 0) {
        fprintf(stderr, "Failed to initialize Wayland surface.\n");
        return -1;
    }

    
    int event_count = 0;
    int target_event_count = 300; 

    while ( event_count < target_event_count && wl_display_dispatch(display) != -1)
    {
        event_count++; // another event processed
        // printf("\r%d, ", event_count);
        // fprintf(stdout, "Wayland event loop iteration\n");
        // fflush(stdout);
        if(state.input.close_flag)
        {
            fprintf(stderr, "Close flag set, exiting Wayland loop.\n");
        }
    }
    wayland_close(&state);

    wayland_cleanup();

    return 0;
}
