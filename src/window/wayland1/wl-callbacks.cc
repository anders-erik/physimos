
#include <linux/input-event-codes.h>

#include "wl-callbacks.hh"

#include "wl-state.hh"



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
    WL_State *state = (WL_State *)data;

    // printf("%s \n", interface);

    if (strcmp(interface, wl_shm_interface.name) == 0)
    {
        state->fb.wl_shm = (wl_shm *)wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
        state->surface_content.fb.wl_shm = (wl_shm *)wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
        state->surface_test.fb.wl_shm = (wl_shm *)wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
    }
    else if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        state->core.compositor = (wl_compositor *)wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);
    }
    else if (strcmp(interface, wl_subcompositor_interface.name) == 0)
    {
        state->core.subcompositor = (wl_subcompositor *)wl_registry_bind(wl_registry, name, &wl_subcompositor_interface, 1);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        state->xdg.wm_base = (xdg_wm_base *)wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1);
    }
    else if (strcmp(interface, wl_seat_interface.name) == 0)
    {
        state->input.seat = (wl_seat *)wl_registry_bind(wl_registry, name, &wl_seat_interface, 1);
    }
}

void registry_global_remove(
    void *data,
    struct wl_registry *registry,
    uint32_t name)
{
    printf("global removed: %u\n", name);
}


void seat_capabilities(
    void *data,
    struct wl_seat *seat,
    uint32_t capabilities)
{
    WL_State* state = (WL_State*) data;

    if (capabilities & WL_SEAT_CAPABILITY_POINTER)
    {
        state->input.pointer = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(state->input.pointer, &state->input.pointer_listener, data);
    }

    if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD)
    {
        state->input.keyboard = wl_seat_get_keyboard(seat);
        wl_keyboard_add_listener(state->input.keyboard,  &state->input.keyboard_listener,  data);
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
    wl_state->input.content_events.push_back(user_input);

    if(wlkey == 1) // Escape
    {
        wl_state->running = 0;
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
    WL_State *state = (WL_State*) data;

    Print::buf("Entered surface: ");


    wl_pointer_set_cursor(pointer, serial, state->cursor.surface, state->cursor.image->hotspot_x, state->cursor.image->hotspot_y);

    if(surface == state->surface_test.surface)
    {
        state->input.target = WL_Input::Test;
        Print::ln("Test!");
    }
    else if(surface == state->surface_content.surface)
    {
        state->input.target = WL_Input::Content;
        Print::ln("Content!");
    }
    else if(surface == state->core.surface_window)
    {
        state->input.target = WL_Input::Window;
        Print::ln("Window!");
    }
    else
    {
        Print::ln("ERROR: entered a non-tracked surface!");
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
    WL_State *state = (WL_State*) data;

    Print::buf("Left surface:    ");

    if(surface == state->surface_test.surface)
    {
        Print::ln("Test!");
    }
    else if(surface == state->surface_content.surface)
    {
        Print::ln("Content!");
    }
    else if(surface == state->core.surface_window)
    {
        Print::ln("Window!");
    }
    else
    {
        Print::ln("ERROR: left a non-tracked surface!");
    }

    state->input.target = WL_Input::None;
}

void
pointer_motion(void *data,
               struct wl_pointer *pointer,
               uint32_t time,
               wl_fixed_t x,
               wl_fixed_t y)
{
    WL_State *state = (WL_State*) data;

    // Print::ln("Pointer motion");

    double px = wl_fixed_to_double(x);
    double py = wl_fixed_to_double(y);

    double win_h = state->fb.h;
    // d2 pos_window_previous_sane = state->input.pointer_pos_window_sane;

    state->input.pointer_pos_window_raw  = {px, py};
    state->input.pointer_pos_window_sane = {px, -( py - win_h) };

    if(state->input.target == WL_Input::Content)
    {
        d2 pos_content_previous_sane = state->input.pointer_pos_content_sane;
        state->input.pointer_pos_content_sane = {   px,
                                                 - (py - state->surface_content.size.y)};

        UserInput pointer_move_ev = { 
            UserInputType::MouseMove, 
            MouseMovement{ pos_content_previous_sane, state->input.pointer_pos_content_sane },
            state->input.key_modifiers
        };
        state->input.content_events.push_back(pointer_move_ev);
    }
    else if(state->input.target == WL_Input::Window)
    {
        Print::ln("Mouse move inside window surface");


        Print::buf("x = ");
        Print::buf(Str::FL(state->input.pointer_pos_window_raw.x, 1, Str::FloatRep::Fixed));
        Print::buf(", y = ");
        Print::buf(Str::FL(state->input.pointer_pos_window_raw.y, 1, Str::FloatRep::Fixed));
        Print::buf("\n");
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
    WL_State *wl_state = (WL_State*) data;
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

    if(wl_state->input.target == WL_Input::Window)
    {
        Print::ln(Str::FL(wl_state->input.pointer_pos_window_raw.y, 2, Str::FloatRep::Fixed));

        enum class WindowArea
        {
            TopLeft,
            Topbar,
            TopRight, 
            RightEdge,
        } window_area;

        if(wl_state->input.pointer_pos_window_raw.y < 20)
        {
            if(wl_state->input.pointer_pos_window_raw.x < 20)
                window_area = WindowArea::TopLeft;
            else if(wl_state->input.pointer_pos_window_raw.x < 800)
                window_area = WindowArea::Topbar;
            else
                window_area = WindowArea::TopRight;
        }
        else
        {
            window_area = WindowArea::RightEdge;
        }

        switch (window_area)
        {
        case WindowArea::TopLeft:
            {
                xdg_toplevel_show_window_menu(wl_state->xdg.toplevel, wl_state->input.seat, serial, 0, 0);
            }
            break;
        case WindowArea::Topbar:
            {
                xdg_toplevel_move(wl_state->xdg.toplevel, wl_state->input.seat, serial);
            }
            break;
        case WindowArea::TopRight:
            {
                // Print::ln("Wayland top right click");
                wl_state->running = 0;
            }
            break;
        case WindowArea::RightEdge:
            {
                xdg_toplevel_resize(wl_state->xdg.toplevel, wl_state->input.seat, serial, xdg_toplevel_resize_edge::XDG_TOPLEVEL_RESIZE_EDGE_RIGHT);
            }
            break;
        
        default:
            break;
        }

        return;
    }
    else if(wl_state->input.target == WL_Input::Content)
    {
        UserInput pointer_click_ev = { 
            UserInputType::MouseClick, 
            mouse_click,
            wl_state->input.key_modifiers
        };
        wl_state->input.content_events.push_back(pointer_click_ev);

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
    WL_State *wl_state = (WL_State*) data;

    ScrollDirection direction;

    if(value > 0)
        direction = ScrollDirection::Down;
    else
        direction = ScrollDirection::Up;
    
    // Print::ln("Axis input");
    // Print::ln(Str::UI(value));

    ScrollInput scroll_input {direction};
    UserInput user_input = {UserInputType::ScrollInput, scroll_input, wl_state->input.key_modifiers};
    wl_state->input.content_events.push_back(user_input);
}
