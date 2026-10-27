
#pragma once

#include <stdio.h>

#include "wayland-client.h"

#include "wl-state.hh"


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
    WL_State* wl_state = (WL_State*) data;

    printf("key: %u, state: %u\n", wlkey, state);

    Keys key;
    KeyButtonAction action = state == 1 ? KeyButtonAction::Press : KeyButtonAction::Release;

    switch (wlkey)
    {
        case 14: key = Keys::Backspace; break;

        case 16: key = Keys::Q; break;
        case 17: key = Keys::W; break;
        case 18: key = Keys::E; break;
        case 19: key = Keys::R; break;
        case 20: key = Keys::T; break;
        case 21: key = Keys::Y; break;
        case 22: key = Keys::U; break;
        case 23: key = Keys::I; break;
        case 24: key = Keys::O; break;
        case 25: key = Keys::P; break;

        case 30: key = Keys::A; break;
        case 31: key = Keys::S; break;
        case 32: key = Keys::D; break;
        case 33: key = Keys::F; break;
        case 34: key = Keys::G; break;
        case 35: key = Keys::H; break;
        case 36: key = Keys::J; break;
        case 37: key = Keys::K; break;
        case 38: key = Keys::L; break;

        case 44: key = Keys::Z; break;
        case 45: key = Keys::X; break;
        case 46: key = Keys::C; break;
        case 47: key = Keys::V; break;
        case 48: key = Keys::B; break;
        case 49: key = Keys::N; break;
        case 50: key = Keys::M; break;

        case 57: key = Keys::Spacebar; break;
    
        default:    break;
    }

    KeyInput key_input = {key, action};
    UserInput user_input = {    UserInputType::KeyInput, 
                                key_input, 
                                wl_state->input.key_modifiers   };
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
    WL_State* state = (WL_State*)data;

    // Print::ln("keyboard modifier callback");

    // This is done because the alt presses were triggering two bits (bit # 4 & 13).
    uint8_t mods_depressed_u8 = (uint8_t) mods_depressed;

    // printf("serial: %d, \n  mods_depressed: %d, \n  mods_latched: %d, \n  mods_locked: %d, \n  group: %d, \n", serial, mods_depressed, mods_latched, mods_locked, group);

    if(mods_depressed_u8 & 0x1)
        state->input.key_modifiers.shift = true;
    else
        state->input.key_modifiers.shift = false;
    
    if(mods_depressed_u8 & 0x4)
        state->input.key_modifiers.control = true;
    else
        state->input.key_modifiers.control = false;
    
    if(mods_depressed_u8 & 0x8)
        state->input.key_modifiers.alt = true;
    else
        state->input.key_modifiers.alt = false;
    

    // state->input.key_modifiers.print();
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