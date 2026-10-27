
#pragma once

#include <stdio.h>

#include "wayland-client.h"

#include "wl-defs.hh"
#include "wl-state.hh"

#include "wl-pointer.hh"
#include "wl-keyboard.hh"



static void seat_capabilities(
    void *data,
    struct wl_seat *seat,
    uint32_t capabilities)
{
    WL_State* state = (WL_State*) data;

    if (capabilities & WL_SEAT_CAPABILITY_POINTER)
    {
        state->input.pointer = wl_seat_get_pointer(seat);

        wl_pointer_add_listener(
            state->input.pointer,
            &pointer_listener,
            data
        );
    }

    if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD)
    {
        state->input.keyboard = wl_seat_get_keyboard(seat);
        wl_keyboard_add_listener(
            state->input.keyboard, 
            &keyboard_listener, 
            data
        );
    }
}


static const struct wl_seat_listener seat_listener = {
    .capabilities = seat_capabilities,
    .name = NULL,
};