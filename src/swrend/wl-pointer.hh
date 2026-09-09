
#pragma once

#include <stdio.h>

#include <linux/input-event-codes.h>

#include "wayland-client.h"

#include "wl-state.hh"


static void
pointer_enter(void *data,
              struct wl_pointer *pointer,
              uint32_t serial,
              struct wl_surface *surface,
              wl_fixed_t x,
              wl_fixed_t y)
{
    print("mouse enter\n");
}

static void
pointer_leave(void *data,
              struct wl_pointer *pointer,
              uint32_t serial,
              struct wl_surface *surface)
{
    print("mouse leave\n");
}

static void
pointer_motion(void *data,
               struct wl_pointer *pointer,
               uint32_t time,
               wl_fixed_t x,
               wl_fixed_t y)
{
    client_state *state = (client_state*) data;

    double px = wl_fixed_to_double(x);
    double py = wl_fixed_to_double(y);

    state->raw_pointer = {px, py};

    double win_w = state->fb.w;
    double win_h = state->fb.h;

    state->sane_pointer.x = px;
    state->sane_pointer.y = win_h - py;



    // printf("mouse: %f %f\n", px, py);
    uint x_uint = (uint) px;
    uint y_uint = (uint) py;

    // printf("mouse: %d %d\n", x_uint, y_uint);
    printf("pointer_sane: %d %d\n", 
        (uint) state->sane_pointer.x, 
        (uint) state->sane_pointer.y);
}

static void
pointer_button(void *data,
               struct wl_pointer *pointer,
               uint32_t serial,
               uint32_t time,
               uint32_t button,
               uint32_t state)
{
    Print::ln(Str::SI(button));

    Str btn_name;
    Str btn_action;
    
    switch(button)
    {
        case BTN_LEFT:
            btn_name = "BTN_LEFT";
            break;

        case BTN_RIGHT:
            btn_name = "BTN_RIGHT";
            break;

         case BTN_MIDDLE:
            btn_name = "BTN_MIDDLE";
            break;

        default:
            break;
    }
    

    switch(state)
    {
        case 0:
            btn_action = "release";
            break;

        case 1:
            btn_action = "press";
            break;

        default:
            break;
    }

    Print::buf("mouse btn: ");
    Print::buf(btn_name);
    Print::buf(" ");
    Print::buf(btn_action);
    Print::buf("\n");
}

static void
pointer_axis(void *data,
             struct wl_pointer *pointer,
             uint32_t time,
             uint32_t axis,
             wl_fixed_t value)
{
}

static const struct wl_pointer_listener pointer_listener = {
    .enter  = pointer_enter,
    .leave  = pointer_leave,
    .motion = pointer_motion,
    .button = pointer_button,
    .axis   = pointer_axis,
};


