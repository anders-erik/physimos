#include <unistd.h>

#include <linux/input-event-codes.h>

#include "lib/str_util.hh"
#include "math/vec.hh"

#include "wl-callbacks.hh"
#include "wayland.hh"
#include "wl-objects.hh"



void
display_error(void *data, struct wl_display *wl_display, void *object_id, uint32_t code, const char *message)
{
    Print::ln("display_error callback");
}
void
display_delete_id(void *data, struct wl_display *wl_display, uint32_t id)
{
    Print::ln("display_delete_id callback");
}


void
registry_global(void *data, struct wl_registry *wl_registry,
        uint32_t name, const char *interface, uint32_t version)
{
    Wayland *wayland = (Wayland*)data;

    // printf("%s \n", interface);

    if (strcmp(interface, wl_shm_interface.name) == 0)
    {
        // state->surfaces.window.fb.wl_shm = (wl_shm *)wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
        // state->surfaces.content.fb.wl_shm = (wl_shm *)wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
        // state->surfaces.test.fb.wl_shm = (wl_shm *)wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
        // state->surfaces.egl.fb.wl_shm = (wl_shm *)wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
        wayland->surfaces.pool.wl_shm = (wl_shm *)wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
    }
    else if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        wayland->core.compositor = (wl_compositor *)wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);
    }
    else if (strcmp(interface, wl_subcompositor_interface.name) == 0)
    {
        wayland->core.subcompositor = (wl_subcompositor *)wl_registry_bind(wl_registry, name, &wl_subcompositor_interface, 1);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        wayland->xdg.wm_base = (xdg_wm_base *)wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1);
    }
    else if (strcmp(interface, wl_seat_interface.name) == 0)
    {
        wayland->input.seat = (wl_seat *)wl_registry_bind(wl_registry, name, &wl_seat_interface, 1);
    }
}

void registry_global_remove(
    void *data,
    struct wl_registry *registry,
    uint32_t name)
{
    printf("global removed: %u\n", name);
}


void wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    Wayland *wayland = (Wayland*)data;
    // Print::ln("wl_buffer_destroy!");
    /* Sent by the compositor when it's no longer using this buffer */
    // wl_buffer_destroy(wl_buffer);


    // Print::buf("wl_buffer released: ");
    // Print::ln(StrUtil::ptr_to_hex(wl_buffer));
    // Print::buf("wl_buffer window : ");
    // Print::ln(StrUtil::ptr_to_hex(state->surfaces.window.buffer));
    // Print::buf("wl_buffer content: ");
    // Print::ln(StrUtil::ptr_to_hex(state->surfaces.content.buffer));
    // Print::buf("wl_buffer test  : ");
    // Print::ln(StrUtil::ptr_to_hex(state->surfaces.test.buffer));

    if(wl_buffer == wayland->surfaces.decorations.surface.buffer)
    {
        // Print::ln("Window buffer released!");
        wayland->surfaces.decorations.surface.surface_is_busy = false;
    }
    else if(wl_buffer == wayland->surfaces.content.buffer)
    {
        // Print::ln("Content buffer released!");
        wayland->surfaces.content.surface_is_busy = false;
    }
    else if(wl_buffer == wayland->surfaces.test.buffer)
    {
        // Print::ln("Test buffer released!");
        wayland->surfaces.test.surface_is_busy = false;
    }
    else
    {
        Print::ln("Unknown buffer released!");
    }

    
}


void seat_capabilities(
    void *data,
    struct wl_seat *seat,
    uint32_t capabilities)
{
    Wayland *wayland = (Wayland*)data;

    if (capabilities & WL_SEAT_CAPABILITY_POINTER)
    {
        wayland->input.pointer = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(wayland->input.pointer, &wayland->input.pointer_listener, data);
    }

    if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD)
    {
        wayland->input.keyboard = wl_seat_get_keyboard(seat);
        wl_keyboard_add_listener(wayland->input.keyboard,  &wayland->input.keyboard_listener,  data);
    }
}



void keyboard_keymap(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t format,
    int32_t fd,
    uint32_t size)
{
    close(fd);  // Important: compositor gives you an FD
}

void keyboard_enter(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t serial,
    struct wl_surface *surface,
    struct wl_array *keys)
{
}

void keyboard_leave(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t serial,
    struct wl_surface *surface)
{
}

void keyboard_key(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t serial,
    uint32_t time,
    uint32_t wlkey,
    uint32_t state)
{
    Wayland *wayland = (Wayland*)data;

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
                                wayland->input.key_modifiers   };
    wayland->input.content_events.push_back(user_input);

    if(wlkey == 1) // Escape
    {
        wayland->running = 0;
        Print::ln("Exit input detected in Wayland lib. State set to not running. Exiting!");
    }
}

void keyboard_modifiers(
    void *data,
    struct wl_keyboard *keyboard,
    uint32_t serial,
    uint32_t mods_depressed,
    uint32_t mods_latched,
    uint32_t mods_locked,
    uint32_t group)
{
    Wayland *wayland = (Wayland*)data;

    // Print::ln("keyboard modifier callback");

    // This is done because the alt presses were triggering two bits (bit # 4 & 13).
    uint8_t mods_depressed_u8 = (uint8_t) mods_depressed;

    // printf("serial: %d, \n  mods_depressed: %d, \n  mods_latched: %d, \n  mods_locked: %d, \n  group: %d, \n", serial, mods_depressed, mods_latched, mods_locked, group);

    if(mods_depressed_u8 & 0x1)
        wayland->input.key_modifiers.shift = true;
    else
        wayland->input.key_modifiers.shift = false;
    
    if(mods_depressed_u8 & 0x4)
        wayland->input.key_modifiers.control = true;
    else
        wayland->input.key_modifiers.control = false;
    
    if(mods_depressed_u8 & 0x8)
        wayland->input.key_modifiers.alt = true;
    else
        wayland->input.key_modifiers.alt = false;
    

    // state->input.key_modifiers.print();
}

void keyboard_repeat_info(
    void *data,
    struct wl_keyboard *keyboard,
    int32_t rate,
    int32_t delay)
{
}







void
pointer_enter(void *data,
              struct wl_pointer *pointer,
              uint32_t serial,
              struct wl_surface *surface,
              wl_fixed_t x,
              wl_fixed_t y)
{
    Wayland *wayland = (Wayland*)data;

    Print::buf("Entered surface: ");

    i2 pointer_pos_raw = {  wl_fixed_to_int(x), wl_fixed_to_int(y) };


    wayland->cursor.pointer_on_last_enter = pointer;
    wayland->cursor.serial_on_last_enter = serial;

    if(surface == wayland->surfaces.test.surface)
    {
        wayland->input.target = WL_Input::Test;
        Print::ln("Test!");
    }
    else if(surface == wayland->surfaces.content.surface)
    {
        wayland->input.target = WL_Input::Content;
        
        wayland->cursor.set_cursor(WL_Cursor::LeftPointer);
        Print::ln("Content!");
    }
    else if(surface == wayland->surfaces.decorations.surface.surface)
    {
        wayland->input.target = WL_Input::Window;

        wayland->surfaces.decorations.area_previous_frame = wayland->surfaces.decorations.area;
        wayland->surfaces.decorations.area = wayland->surfaces.decorations.get_area(pointer_pos_raw);

        
        wayland->cursor.set_cursor(WL_Cursor::decoration_area_to_cursor_type(wayland->surfaces.decorations.area));

        // switch (wayland->surfaces.decorations.area)
        // {
        // case WLDecorationArea::EdgeRight:
        //         wayland->cursor.set_cursor(WL_Cursor::ResizeEastWest);
        //     break;
        // case WLDecorationArea::EdgeTopLeft:
        //         wayland->cursor.set_cursor(WL_Cursor::ResizeNESW);
        //     break;
        // case WLDecorationArea::EdgeTop:
        //         wayland->cursor.set_cursor(WL_Cursor::ResizeNorthSouth);
        //     break;
        // case WLDecorationArea::EdgeTopRight:
        //         wayland->cursor.set_cursor(WL_Cursor::ResizeNESW);
        //     break;
        // case WLDecorationArea::EdgeBottom:
        //         wayland->cursor.set_cursor(WL_Cursor::ResizeNorthSouth);
        //     break;
        // case WLDecorationArea::Topbar:
        //         wayland->cursor.set_cursor(WL_Cursor::LeftPointer);
        //     break;
        
        // default:
        //         wayland->cursor.set_cursor(WL_Cursor::None);
        //     break;
        // }

        Print::ln("Window!");
    }
    else if(surface == wayland->surfaces.egl.surface)
    {
        wayland->input.target = WL_Input::EGL;
        Print::ln("EGL!");
    }
    else
    {
        Print::ln("Unknown!");
    }

    

    // pointer_serial = serial;

    

    // wl_pointer_set_cursor(
    //     pointer,
    //     serial,
    //     cursor_surface,
    //     hotspot_x,
    //     hotspot_y
    // );
    // wl_surface_set_user_data(
}

void
pointer_leave(void *data,
              struct wl_pointer *pointer,
              uint32_t serial,
              struct wl_surface *surface)
{
    Wayland *wayland = (Wayland*)data;

    Print::buf("Left surface:    ");

    if(surface == wayland->surfaces.test.surface)
    {
        Print::ln("Test!");
    }
    else if(surface == wayland->surfaces.content.surface)
    {
        Print::ln("Content!");
    }
    else if(surface == wayland->surfaces.decorations.surface.surface)
    {
        Print::ln("Window!");
    }
    else if(surface == wayland->surfaces.egl.surface)
    {
        Print::ln("EGL!");
    }
    else
    {
        Print::ln("Unknown!");
    }

    wayland->input.target = WL_Input::None;
}

void
pointer_motion(void *data,
               struct wl_pointer *pointer,
               uint32_t time,
               wl_fixed_t x,
               wl_fixed_t y)
{
   Wayland *wayland = (Wayland*)data;

    // Print::ln("Pointer motion");

    double px = wl_fixed_to_double(x);
    double py = wl_fixed_to_double(y);

    double win_h = wayland->surfaces.decorations.surface.pixel_buffer.h;
    // d2 pos_window_previous_sane = wayland->input.pointer_pos_window_sane;

    wayland->input.pointer_pos_window_raw  = {px, py};
    wayland->input.pointer_pos_window_sane = {px, -( py - win_h) };

    if(wayland->input.target == WL_Input::Content)
    {
        // wayland->cursor.set_cursor(WL_Cursor::LeftPointer);

        d2 pos_content_previous_sane = wayland->input.pointer_pos_content_sane;
        wayland->input.pointer_pos_content_sane = {   px,
                                                 - (py - wayland->surfaces.content.pixel_buffer.h)};

        UserInput pointer_move_ev = { 
            UserInputType::MouseMove, 
            MouseMovement{ pos_content_previous_sane, wayland->input.pointer_pos_content_sane },
            wayland->input.key_modifiers
        };
        wayland->input.content_events.push_back(pointer_move_ev);
    }
    else if(wayland->input.target == WL_Input::Window)
    {
        wayland->surfaces.decorations.area_previous_frame = wayland->surfaces.decorations.area;
        wayland->surfaces.decorations.area = wayland->surfaces.decorations.get_area({   wayland->input.pointer_pos_window_raw.x, 
                                                                                        wayland->input.pointer_pos_window_raw.y });

        print_area_name(wayland->surfaces.decorations.area);

        if(wayland->surfaces.decorations.area_previous_frame != wayland->surfaces.decorations.area)
        {
            WL_Cursor::Type cursor_type = WL_Cursor::decoration_area_to_cursor_type(wayland->surfaces.decorations.area);
            wayland->cursor.set_cursor(cursor_type);

        }

        

        // Print::buf("x = ");
        // Print::buf(Str::FL(wayland->input.pointer_pos_window_raw.x, 1, Str::FloatRep::Fixed));
        // Print::buf(", y = ");
        // Print::buf(Str::FL(wayland->input.pointer_pos_window_raw.y, 1, Str::FloatRep::Fixed));
        // Print::buf("\n");
    }

    
}

void
pointer_button(void *data,
               struct wl_pointer *pointer,
               uint32_t serial,
               uint32_t time,
               uint32_t button,
               uint32_t state)
{
    Wayland *wayland = (Wayland*)data;
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

    if(wayland->input.target == WL_Input::Window)
    {
        Print::ln(Str::FL(wayland->input.pointer_pos_window_raw.y, 2, Str::FloatRep::Fixed));

        // WL_Decoration::Area area = wayland->surfaces.decorations.get_area({wayland->input.pointer_pos_window_raw.x, wayland->input.pointer_pos_window_raw.y});
        

        switch (wayland->surfaces.decorations.area)
        {
        case WLDecorationArea::TopLeft:
            {
                xdg_toplevel_show_window_menu(wayland->xdg.toplevel, wayland->input.seat, serial, 0, 0);
            }
            break;
        case WLDecorationArea::Topbar:
            {
                xdg_toplevel_move(wayland->xdg.toplevel, wayland->input.seat, serial);
            }
            break;
        case WLDecorationArea::CloseButton:
            {
                // Print::ln("Wayland top right click");
                wayland->running = 0;
            }
            break;
        case WLDecorationArea::MinimizeButton:
            {
                Print::ln("Minimize button clicked!");
            }
            break;
        case WLDecorationArea::MaximizeButton:
            {
                Print::ln("Maximize button clicked!");
            }
            break;
        case WLDecorationArea::EdgeTop:
            {
                xdg_toplevel_resize(wayland->xdg.toplevel, wayland->input.seat, serial, xdg_toplevel_resize_edge::XDG_TOPLEVEL_RESIZE_EDGE_TOP);
            }
            break;
        case WLDecorationArea::EdgeRight:
            {
                xdg_toplevel_resize(wayland->xdg.toplevel, wayland->input.seat, serial, xdg_toplevel_resize_edge::XDG_TOPLEVEL_RESIZE_EDGE_RIGHT);
            }
            break;
        case WLDecorationArea::EdgeBottom:
            {
                xdg_toplevel_resize(wayland->xdg.toplevel, wayland->input.seat, serial, xdg_toplevel_resize_edge::XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM);
            }
            break;
        
        default:
            break;
        }

        return;
    }
    else if(wayland->input.target == WL_Input::Content)
    {
        UserInput pointer_click_ev = { 
            UserInputType::MouseClick, 
            mouse_click,
            wayland->input.key_modifiers
        };
        wayland->input.content_events.push_back(pointer_click_ev);

        // Print::buf("mouse btn: ");
        // Print::buf(btn_name);
        // Print::buf(" ");
        // Print::buf(btn_action);
        // Print::buf("\n");
    }
    

    


    
}

void
pointer_axis(void *data,
             struct wl_pointer *pointer,
             uint32_t time,
             uint32_t axis,
             wl_fixed_t value)
{
    Wayland *wayland = (Wayland*)data;

    ScrollDirection direction;

    if(value > 0)
        direction = ScrollDirection::Down;
    else
        direction = ScrollDirection::Up;
    
    // Print::ln("Axis input");
    // Print::ln(Str::UI(value));

    ScrollInput scroll_input {direction};
    UserInput user_input = {UserInputType::ScrollInput, scroll_input, wayland->input.key_modifiers};
    wayland->input.content_events.push_back(user_input);
}





void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}



void
xdg_surface_configure(  void *data,
                        struct xdg_surface *xdg_surface,
                        uint32_t serial                     )
{
    // struct client_state *state = (struct client_state *)data;
    xdg_surface_ack_configure(xdg_surface, serial);

    // Print::buf("serial: ");
    // Print::buf(Str::UI(serial));
    // Print::buf("\n");

    Print::ln("xdg_surface_configure called!");

    // render_wayland(data);
}




void
xdg_toplevel_configure( void *data,
                        struct xdg_toplevel *xdg_toplevel,
                        int32_t width,
                        int32_t height,
                        struct wl_array *states                     )
{
    Wayland *wayland = (Wayland*)data;
    
    

    

    // Print::ln({Str{"new_width = "}, Str::SI(width), Str{",  new_height = "}, Str::SI(height)});

    Vec<uint32_t> state_array {states->size, (uint32_t*)states->data};

    uint32_t* array_data = (uint32_t*)states->data;
    size_t number_of_values = states->size / 4;

    Print::buf("xdg_toplevel_configure called with ");
    Print::buf(Str::SI(number_of_values));
    Print::buf(" number of values.");
    Print::ln("");

    for(size_t i = 0; i < number_of_values ; i++)
    {
        Print::buf("Value ");
        Print::buf(Str::UI( i ));
        Print::buf(" = '");
        Print::buf(Str::SI( state_array[i] ));
        Print::ln("'");
    }

    bool proper_resize_event = (state_array[0] == 3) && (state_array[1] == 4);
    if(proper_resize_event)
    {
        wayland->surfaces.window_size_change_detected = true;
        wayland->surfaces.size_change_dims = {width, height};

        Print::buf("Wayland window size change detected. New size = { ");
        Print::buf(Str::SI(wayland->surfaces.size_change_dims.x));
        Print::buf(", ");
        Print::buf(Str::SI(wayland->surfaces.size_change_dims.y));
        Print::ln(" }");
    }

    
    
    // Print::ln(Str::SI( *(array_data + 0) ));
    // Print::ln(Str::SI( *(array_data + 1) ));
    // Print::ln(Str::SI( *(array_data + 2) ));
    // Print::ln(Str::SI( *((((int32_t)states->data))) ));

    // xdg_toplevel_state state = wl_array_for_each(0, states);
    // uint8_t *key;
    // wl_array_for_each(key, (uint8_t *)states)
    // {
    //     // Print::ln(Str::SI(*key));
    //     Print::ln("toplevel configure loop");
    // }
}

void
xdg_toplevel_close( void *data,
		            struct xdg_toplevel *xdg_toplevel)
{
    // Print::ln("xdg_toplevel_close called!");
}

void
xdg_toplevel_configure_bounds(  void *data,
                                struct xdg_toplevel *xdg_toplevel,
                                int32_t width,
                                int32_t height)
{
    // Print::ln("xdg_toplevel_configure_bounds called!");
}

void
xdg_toplevel_wm_capabilities(   void *data,
                                struct xdg_toplevel *xdg_toplevel,
                                struct wl_array *capabilities)
{
    // Print::ln("xdg_toplevel_wm_capabilities called!");
}