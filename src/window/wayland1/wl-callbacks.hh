#pragma once

#include "wayland-client.h"

#include "wl-defs.hh"

// DISPLAY
extern void
display_error(void *data, struct wl_display *wl_display, void *object_id, uint32_t code, const char *message);
extern void
display_delete_id(void *data, struct wl_display *wl_display, uint32_t id);

// GLOBAL
extern void
registry_global(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version);
extern void
registry_global_remove( void *data, struct wl_registry *registry, uint32_t name);

// BUFFER
extern void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer);

// SEAT
extern void 
seat_capabilities(void *data, struct wl_seat *seat, uint32_t capabilities);

// KEYBOARD
extern void 
keyboard_keymap(void *data, struct wl_keyboard *keyboard, uint32_t format, int32_t fd, uint32_t size);
extern void 
keyboard_enter(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface, struct wl_array *keys);
extern void 
keyboard_leave(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface);
extern void 
keyboard_key(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t wlkey, uint32_t state);
extern void 
keyboard_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
extern void 
keyboard_repeat_info(void *data, struct wl_keyboard *keyboard, int32_t rate, int32_t delay);

// POINTER
extern void
pointer_enter(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t x, wl_fixed_t y);
extern void
pointer_leave(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface);
extern void
pointer_motion(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y);
extern void
pointer_button(void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
extern void
pointer_axis(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value);







// XDG

extern void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial);

extern void
xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial);

extern void
xdg_toplevel_configure( void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states );
extern void
xdg_toplevel_close(void *data, struct xdg_toplevel *xdg_toplevel);
extern void
xdg_toplevel_configure_bounds(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height);
extern void
xdg_toplevel_wm_capabilities(void *data, struct xdg_toplevel *xdg_toplevel, struct wl_array *capabilities);


