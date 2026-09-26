
#pragma once

#include <stdio.h>

#include "wayland-client.h"

// static void keyboard_key(
//     void *data,
//     struct wl_keyboard *keyboard,
//     uint32_t serial,
//     uint32_t time,
//     uint32_t key,
//     uint32_t state)
// {
//     printf("key=%u state=%u\n", key, state);
// }


static void keyboard_keymap(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t format,
    int32_t fd,
    uint32_t size)
{
    close(fd);  // Important: compositor gives you an FD
}

static void keyboard_enter(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t serial,
    struct wl_surface *surface,
    struct wl_array *keys)
{
}

static void keyboard_leave(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t serial,
    struct wl_surface *surface)
{
}

static void keyboard_key(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t serial,
    uint32_t time,
    uint32_t wlkey,
    uint32_t state)
{
    WaylandState* wl_state = (WaylandState*) data;

    printf("key: %u, state: %u\n", wlkey, state);

    Keys key;
    KeyButtonAction action = state == 1 ? KeyButtonAction::Press : KeyButtonAction::Release;

    switch (wlkey)
    {
        case 14: key = Keys::Backspace; break;

        case 30: key = Keys::A; break;
        case 31: key = Keys::S; break;
        case 32: key = Keys::D; break;
        case 33: key = Keys::F; break;
        case 34: key = Keys::G; break;
        case 35: key = Keys::H; break;
        case 36: key = Keys::J; break;
        case 37: key = Keys::K; break;
        case 38: key = Keys::L; break;
    
        default:    break;
    }

    KeyPress key_press = {key, action};
    UserInput user_input = { UserInputType::KeyPress, key_press};
    wl_state->input.w_events.push_back(user_input);

    if(wlkey == 1) // Escape
    {
        wl_state->running = 0;
        Print::ln("Exit input detected in Wayland lib. State set to not running. Exiting!");
    }
}

static void keyboard_modifiers(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t serial,
    uint32_t mods_depressed,
    uint32_t mods_latched,
    uint32_t mods_locked,
    uint32_t group)
{
}

static void keyboard_repeat_info(
    void *data,
    struct wl_keyboard *keyboard,
    int32_t rate,
    int32_t delay)
{
}

static const struct wl_keyboard_listener keyboard_listener = {
    .keymap      = keyboard_keymap,
    .enter       = keyboard_enter,
    .leave       = keyboard_leave,
    .key         = keyboard_key,
    .modifiers   = keyboard_modifiers,
    .repeat_info = keyboard_repeat_info,
};