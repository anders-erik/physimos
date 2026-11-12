#pragma once


#include "lib/arr.hh"
#include "lib/llist.hh"
#include "lib/pixel_buffer.hh"

#include "io/input/user_input.hh"

#include "wl-defs.hh"
#include "wl-callbacks.hh"

#include "wl-egl.hh"


struct WL_Cursor
{
    struct wl_cursor        *cursor;
    struct wl_surface       *surface;
    struct wl_cursor_image  *image;
    struct wl_cursor_theme  *theme;
    struct wl_buffer        *buffer;

    struct wl_pointer *pointer_last_callback;
    uint32_t serial_enter;

    enum Type
    {
        LeftPointer,
        ResizeEastWest,
        ResizeNorthSouth,
        ResizeNESW, // North-East & South-West
        None,
    };

    void init(wl_compositor* _compositor, wl_shm* _pool_shm)
    {
        surface = wl_compositor_create_surface(_compositor);

        // CREATE CURSOR
        theme = wl_cursor_theme_load(NULL, 24, _pool_shm);

        // cursor = wl_cursor_theme_get_cursor(theme, "left_ptr");
        // cursor = wl_cursor_theme_get_cursor(theme, "right_ptr");
        // cursor = wl_cursor_theme_get_cursor(theme, "crosshair");
        // cursor = wl_cursor_theme_get_cursor(theme, "left_ptr_watch");
        // cursor = wl_cursor_theme_get_cursor(theme, "crossed_circle");
        // cursor = wl_cursor_theme_get_cursor(theme, "h_double_arrow");
        // cursor = wl_cursor_theme_get_cursor(theme, "v_double_arrow");
        // cursor = wl_cursor_theme_get_cursor(theme, "bd_double_arrow");
        // cursor = wl_cursor_theme_get_cursor(theme, "fd_double_arrow");
        
        
        // reload_cursor();
    }

  

    void set_cursor(WL_Cursor::Type type)
    {
        switch (type)
        {
        case WL_Cursor::LeftPointer:
            {
            cursor = wl_cursor_theme_get_cursor(theme, "left_ptr");
            Print::ln("left_ptr");
            }
            break;
        case WL_Cursor::ResizeEastWest:
            {
            cursor = wl_cursor_theme_get_cursor(theme, "h_double_arrow");
            Print::ln("h_double_arrow");
            }
            break;
        case WL_Cursor::ResizeNorthSouth:
            {
            cursor = wl_cursor_theme_get_cursor(theme, "v_double_arrow");
            Print::ln("v_double_arrow");
            }
            break;
        case WL_Cursor::ResizeNESW:
            {
            cursor = wl_cursor_theme_get_cursor(theme, "fd_double_arrow");
            Print::ln("fd_double_arrow");
            }
            break;
        case WL_Cursor::None:
            {
            wl_pointer_set_cursor(pointer_last_callback, serial_enter, NULL, image->hotspot_x, image->hotspot_y);
            Print::ln("No cursor image");
            return;
            }
            break;
        
        default:
            break;
        }

        image = cursor->images[0];
        buffer = wl_cursor_image_get_buffer(image);
        wl_surface_attach(surface, buffer, 0, 0);
        wl_surface_damage_buffer(surface, 0, 0, 10, 10); // PROBELM SOLVED
        wl_surface_commit(surface);
        wl_pointer_set_cursor(pointer_last_callback, serial_enter, surface, image->hotspot_x, image->hotspot_y);
        Print::ln(Str::UI(serial_enter));
        // Print::ln(Str::UI(pointer_last_callback));
        printf("The value of s is: %p\n", (void *) pointer_last_callback);
    }


};



/** Wayland core objects */
struct WL_Core
{
    struct wl_display *display;
    const struct wl_display_listener display_listener = {
        .error      = display_error,
        .delete_id  = display_delete_id,
    };
    struct wl_registry *registry;
    const struct wl_registry_listener registry_listener = {
        .global        = registry_global,
        .global_remove = registry_global_remove,
    };
    struct wl_compositor *compositor;
    struct wl_subcompositor *subcompositor;
};

/** XDG objects */
struct WL_XDG
{
    struct xdg_wm_base *wm_base;
    const struct xdg_wm_base_listener xdg_wm_base_listener = {
        .ping = xdg_wm_base_ping,
    };
    struct xdg_surface *surface;
    const struct xdg_surface_listener xdg_surface_listener = {
        .configure = xdg_surface_configure,
    };
    struct xdg_toplevel *toplevel;
    const struct xdg_toplevel_listener xdg_toplevel_listener = {
        .configure = xdg_toplevel_configure,
        .close = xdg_toplevel_close,
        .configure_bounds = xdg_toplevel_configure_bounds,
        .wm_capabilities = xdg_toplevel_wm_capabilities,
    };
};

struct WL_Input
{
    struct wl_seat *seat;
    const struct wl_seat_listener seat_listener = {
        .capabilities = seat_capabilities,
        .name = NULL,
    };

    struct wl_pointer* pointer = NULL;
    const struct wl_pointer_listener pointer_listener = {
        .enter  = pointer_enter,
        .leave  = pointer_leave,
        .motion = pointer_motion,
        .button = pointer_button,
        .axis   = pointer_axis,
    };
    struct wl_keyboard* keyboard = NULL;
    const struct wl_keyboard_listener keyboard_listener = {
        .keymap      = keyboard_keymap,
        .enter       = keyboard_enter,
        .leave       = keyboard_leave,
        .key         = keyboard_key,
        .modifiers   = keyboard_modifiers,
        .repeat_info = keyboard_repeat_info,
    };

    enum Target
    {
        Window,
        Content,
        Test,
        EGL,
        None,
    } target = None;

    // struct wl_surface *cursor_surface;
    // struct wl_cursor_image *cursor_image;

    Arr<UserInput> content_events;

    KeyModifiers key_modifiers;

    // Pointer state
    d2 pointer_pos_window_raw = {0.0, 0.0};
    d2 pointer_pos_window_sane = {0.0, 0.0};

    // d2 pointer_pos_content_raw = {0.0, 0.0};
    d2 pointer_pos_content_sane = {0.0, 0.0};
    // d2 pointer_sane = {0.0, 0.0};
};




