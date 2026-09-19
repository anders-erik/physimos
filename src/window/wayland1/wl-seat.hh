
#pragma once

#include <stdio.h>

#include "wayland-client.h"


#include "wl-pointer.hh"
#include "wl-keyboard.hh"


static struct wl_pointer *pointer = NULL;
static struct wl_keyboard *kbd_ptr = NULL;

static void seat_capabilities(
    void *data,
    struct wl_seat *seat,
    uint32_t capabilities)
{
    if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
        pointer = wl_seat_get_pointer(seat);

        wl_pointer_add_listener(
            pointer,
            &pointer_listener,
            data
        );
    }

    if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
        kbd_ptr = wl_seat_get_keyboard(seat);
        wl_keyboard_add_listener(
            kbd_ptr, 
            &keyboard_listener, 
            data
        );
    }
}


static const struct wl_seat_listener seat_listener = {
    .capabilities = seat_capabilities,
    .name = NULL,
};