#pragma once

// #include "wayland-client.h"
#include "wl-defs.hh"


struct WL_Pool
{
    struct wl_shm *wl_shm = nullptr;
    struct wl_shm_pool *pool = nullptr;
    int shm_fd = -1;
    int size = 0;
    void* data = nullptr;

    void randname(char *buf);
    int create_shm_file();
    int allocate_shm_file(int size);

    void init(int _size);
};