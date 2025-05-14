#define _POSIX_C_SOURCE 200112L
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <wayland-client.h>
#include "protocols/xdg-shell-client-protocol.h"

static struct wl_display* display;
static struct wl_registry* registry;
static struct wl_compositor* compositor;
static struct xdg_wm_base* wm_base;
static struct wl_shm* shm;

static struct wl_surface* surface;
static struct xdg_surface* xdg_surface;
static struct xdg_toplevel* toplevel;
static struct wl_buffer* buffer;

static void shm_format(void* data, struct wl_shm* shm, uint32_t format) {}

static const struct wl_shm_listener shm_listener = {
    .format = shm_format
};

static void global_handler(void* data, struct wl_registry* reg, uint32_t id,
                           const char* interface, uint32_t version) {
    if (strcmp(interface, wl_compositor_interface.name) == 0)
        compositor = wl_registry_bind(reg, id, &wl_compositor_interface, 4);
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        wm_base = wl_registry_bind(reg, id, &xdg_wm_base_interface, 1);
    else if (strcmp(interface, wl_shm_interface.name) == 0) {
        shm = wl_registry_bind(reg, id, &wl_shm_interface, 1);
        wl_shm_add_listener(shm, &shm_listener, NULL);
    }
}

static void global_remove(void* data, struct wl_registry* reg, uint32_t id) {}

static const struct wl_registry_listener registry_listener = {
    .global = global_handler,
    .global_remove = global_remove
};

static void xdg_ping(void* data, struct xdg_wm_base* wm, uint32_t serial) {
    xdg_wm_base_pong(wm, serial);
}

static const struct xdg_wm_base_listener wm_base_listener = {
    .ping = xdg_ping
};

static void xdg_surface_configure(void* data, struct xdg_surface* s, uint32_t serial) {
    xdg_surface_ack_configure(s, serial);
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure
};

// Creates a shm buffer of given size and returns a wl_buffer
static struct wl_buffer* create_buffer(int width, int height) {
    int stride = width * 4;
    int size = stride * height;

    char filename[] = "/tmp/wayland-shm-XXXXXX";
    int fd = mkstemp(filename);
    unlink(filename); // unlinked immediately

    ftruncate(fd, size);
    uint32_t* data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Fill with black
    memset(data, 0, size);

    struct wl_shm_pool* pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer* buf = wl_shm_pool_create_buffer(pool, 0,
        width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    munmap(data, size);
    close(fd);
    return buf;
}

int main() {
    display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "No Wayland display.\n");
        return 1;
    }

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);

    if (!compositor || !wm_base || !shm) {
        fprintf(stderr, "Missing required Wayland globals.\n");
        return 1;
    }

    xdg_wm_base_add_listener(wm_base, &wm_base_listener, NULL);

    surface = wl_compositor_create_surface(compositor);
    xdg_surface = xdg_wm_base_get_xdg_surface(wm_base, surface);
    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, NULL);

    toplevel = xdg_surface_get_toplevel(xdg_surface);
    xdg_toplevel_set_title(toplevel, "Wayland shm window");

    buffer = create_buffer(100, 100); // 100x100 black

    wl_surface_commit(surface);

    while (wl_display_dispatch(display) != -1) {}
    return 0;
}
