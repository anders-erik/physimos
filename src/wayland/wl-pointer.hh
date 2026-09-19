
#pragma once

#include <stdio.h>

#include <linux/input-event-codes.h>

#include "wayland-client.h"

// #include "wl-state.hh"


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
    state->sane_pointer.x = px;
    state->sane_pointer.y = state->fb.h - py;

    // printf("mouse: %d %d\n", x_uint, y_uint);
    // printf("pointer_sane: %d %d\n", 
    //     (uint) state->sane_pointer.x, 
    //     (uint) state->sane_pointer.y);


    // ----- New below -----

    double win_h = state->fb.h;

    d2 pos_old_sane = state->input.pointer_pos_last_sane;
    d2 pos_current_sane = {px, -( py - win_h) };

    state->input.pointer_pos_last_sane = pos_current_sane;

    
    UserInput pointer_move_ev = { 
        UserInputType::MouseMove, 
        MouseMovement{ pos_old_sane, pos_current_sane }
    };
    state->input.w_events.push_back(pointer_move_ev);
}

static void
pointer_button(void *data,
               struct wl_pointer *pointer,
               uint32_t serial,
               uint32_t time,
               uint32_t button,
               uint32_t state)
{
    client_state *w_app = (client_state*) data;
    // struct client_state *state = (struct client_state*) data;

    // Print::ln(Str::SI(button));

    Str btn_name;
    Str btn_action;

    // WEventType event_type = WEventType::MouseClick;
    // WEventButtonAction button_action;
    MouseClick mouse_click;
    
    switch(button)
    {
        case BTN_LEFT:
            btn_name = "BTN_LEFT";
            mouse_click.button = MouseClick::Primary;
            break;

        case BTN_RIGHT:
            btn_name = "BTN_RIGHT";
            mouse_click.button = MouseClick::Secondary;
            break;

         case BTN_MIDDLE:
            btn_name = "BTN_MIDDLE";
            mouse_click.button = MouseClick::Tertiary;
            break;

        default:
            break;
    }
    

    switch(state)
    {
        case 0:
            btn_action = "release";
            mouse_click.action = MouseButtonAction::Release;
            break;

        case 1:
            btn_action = "press";
            mouse_click.action = MouseButtonAction::Press;
            break;

        default:
            break;
    }


    UserInput pointer_click_ev = { 
        UserInputType::MouseClick, 
        mouse_click
    };
    w_app->input.w_events.push_back(pointer_click_ev);

    // Print::buf("mouse btn: ");
    // Print::buf(btn_name);
    // Print::buf(" ");
    // Print::buf(btn_action);
    // Print::buf("\n");
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


