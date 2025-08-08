#pragma once

#include "w_types.hh"

#include <wayland-client.h>

#include <stdio.h>
#include <unistd.h>

#include <linux/input-event-codes.h>



#ifdef  __cplusplus
extern "C" {
#endif




void empty_fn() {}

void pointer_leave(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface) {}

void pointer_enter(void *data, struct wl_pointer *pointer, uint32_t serial,
                   struct wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy)
{
    WState *state = (WState *)data;

    fprintf(    stderr,
                "Pointer entered surface at (%f, %f)\n", 
                wl_fixed_to_double(sx), 
                wl_fixed_to_double(sy)                  );
    // set default pointer
    // struct wl_cursor *cursor = wl_cursor_theme_get_cursor(cursor_theme, "left_ptr");
    // wl_pointer_set_cursor(pointer, serial, /* TODO */);
    wl_pointer_set_cursor(  pointer, 
                            serial, 
                            state->input.cursor_surface,
                            state->input.cursor_image->hotspot_x, 
                            state->input.cursor_image->hotspot_y);
}
void pointer_motion(void *data, struct wl_pointer *pointer, uint32_t time,
                    wl_fixed_t sx, wl_fixed_t sy)
{
    fprintf(stderr, "Pointer moved to (%f, %f) at time %u\n", 
            wl_fixed_to_double(sx), 
            wl_fixed_to_double(sy), 
            time);
}
void pointer_button(void *data, struct wl_pointer *pointer, uint32_t serial,
                    uint32_t time, uint32_t button, uint32_t state)
{
    fprintf(stderr, "Pointer button %u %s at time %u\n", 
            button, 
            state ? "pressed" : "released", 
            time);
    
    if(button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED)
    {
        fprintf(stdout, "Left button pressed\n");
        // state.input.close_flag = 1; // Example action
    }
}
void pointer_axis(void *data, struct wl_pointer *pointer, uint32_t time,
                  uint32_t axis, wl_fixed_t value)
{
    fprintf(stderr, "Pointer axis %u moved by %f at time %u\n", 
            axis, 
            wl_fixed_to_double(value), 
            time);
}

static const struct wl_pointer_listener pointer_listener = {
    .enter  = pointer_enter,
    .leave  = pointer_leave,
    .motion = pointer_motion,
    .button = pointer_button,
    .axis   = pointer_axis,
};




void keyboard_keymap(void *data, struct wl_keyboard *keyboard, uint32_t format,
                  int fd, uint32_t size)
{
    fprintf(stderr, "Keyboard keymap received: format %u, fd %d, size %u\n", format, fd, size);
    close(fd); // Close the file descriptor after use
}

void keyboard_enter(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                  struct wl_surface *surface, struct wl_array *keys)
{
    fprintf(stderr, "Keyboard entered surface\n");
}
void keyboard_leave(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                  struct wl_surface *surface)
{
    fprintf(stderr, "Keyboard left surface\n");
}

void keyboard_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                       uint32_t mods_depressed, uint32_t mods_latched,
                       uint32_t mods_locked, uint32_t group)
{
    fprintf(stderr, "Keyboard modifiers changed: depressed %u, latched %u, locked %u, group %u\n",
            mods_depressed, mods_latched, mods_locked, group);
}

void keyboard_repeat_info(void *data, struct wl_keyboard *keyboard, int32_t rate,
                         int32_t delay)
{
    fprintf(stderr, "Keyboard repeat info: rate %d, delay %d\n", rate, delay);
}

void keyboard_key(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                  uint32_t time, uint32_t key, uint32_t state_)
{
    fprintf(stderr, "Keyboard key %u %s at time %u\n", key,
            state_ ? "pressed" : "released", time);
    
    if(key == KEY_ESC)
    {
        fprintf(stdout, "ESC\n");
        // state.input.close_flag = 1;
    }
}

static const struct wl_keyboard_listener keyboard_listener = {
    .keymap = keyboard_keymap,
    .enter = keyboard_enter,
    .leave = keyboard_leave,
    .key = keyboard_key,
    .modifiers = keyboard_modifiers,
    .repeat_info = keyboard_repeat_info
};




#ifdef  __cplusplus
}
#endif