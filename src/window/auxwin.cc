
#include <iostream>

#include <glad/glad.h>

#if PHY_NATWIN == 1
#include <natwin/natwin.hh>
#else
#include "window/glfw.hh"
#endif

#include "print.hh"

#include "opengl/texture.hh"
#include "opengl/texture_unit.hh"

#include "auxwin.hh"


Auxwin* get_current_auxwin()
{
    return current_auxwin;
}


namespace window
{

// Auxwin* current_auxwin;



void CursorPosition::print()
{
    std::cout << std::endl;
    input.print("Input       ");
    sane.print("Sane        ");
    pixels.print("Pixel       ");
    normalized.print("Normalized  ");
    millimeters.print("Millimeters ");
}





void Auxwin::init(int width, int height)
{
    current_auxwin = this;

#ifdef PHY_NATWIN
    std::cout << "Natwin is not implemented yet. Exiting." << std::endl;
    exit(1);
#else
    pwglfw.init({width, height});
#endif

    gl.init(get_auxwin_proc_adr_fn());

    reload_coordinate_constants();    
}




void Auxwin::reload_coordinate_constants()
{
    coords_input.monitor_size_px = get_monitor_size_px();
    coords_input.monitor_size_mm = get_monitor_size_mm();
    coords_input.content_scale = get_monitor_content_scale();
    coords_input.window_size_sc = get_window_fb_size_float() / coords_input.content_scale;

    coord_transformer.set_constants(coords_input);
}



AuxwinProcAdrFn Auxwin::get_auxwin_proc_adr_fn()
{

#if PHY_NATWIN == 1

#else 
    return (AuxwinProcAdrFn) glfwGetProcAddress;
#endif

}



void Auxwin::make_current()
{
    pwglfw.make_current();
}


void Auxwin::
bind_window_framebuffer()
{
    glBindFramebuffer(  GL_FRAMEBUFFER, 
                        0               );

    pwglfw.set_window_viewport();
}




void Auxwin::
try_close_keystroke(KeyStrokeEvent keystroke, KeyModifiers modifiers)
{
    if( keystroke.is(ButtonAction::Press)   &&
        keystroke.key == Key::F1            &&
        modifiers.is_esc()               )
    {
            close();
    }

    if( keystroke.is(ButtonAction::Press)   &&
        keystroke.key == Key::q             &&
        modifiers.is_alt_ctrl_shift()           )
    {
            close();
    }
}



void Auxwin::
close(){
    pwglfw.close();
    open = false;
}


bool Auxwin::
is_open()
{
    return pwglfw.is_open();
}


void Auxwin::new_frame()
{
    timer.stop();

    for(int i=1; i<dt_count; i++)
        dt_s_last_10[i-1] = dt_s_last_10[i];
    dt_s_last_10[dt_count-1] = timer.get_s();
    timer.start();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    // glClear(GL_COLOR_BUFFER_BIT);
    // glEnable(GL_CULL_FACE);

}

bool Auxwin::end_frame()
{
    glBindVertexArray(0);

    pwglfw.end_frame();
    
    return pwglfw.is_open();
}


void Auxwin::destroy()
{
    pwglfw.destroy();
}



void Auxwin::
set_cursor_state(Cursor::State new_state)
{
    // Prevent unnecesary cursor calls
    if(cursor.state == new_state)
        return;

    cursor.state = new_state;

    pwglfw.set_cursor_state(new_state);

}


f2 Auxwin::get_center_pos()
{
    return (get_window_fb_size_float() * get_monitor_content_scale()) * 0.5f;
}


CursorPosition Auxwin::
get_cursor_pos() const
{
    return cursor_pos;
}


i2 Auxwin::
get_window_fb_size() const
{
    return pwglfw.dims.fb_size_i;
}

f2 Auxwin::
get_window_fb_size_float() const
{
    return pwglfw.dims.fb_size_i.to_f2();
}



f2 Auxwin::get_monitor_content_scale()
{
    return pwglfw.get_content_scale();
}

f2 Auxwin::get_monitor_size_px()
{
    return pwglfw.get_monitor_size_px().to_f2();
}

f2 Auxwin::get_monitor_size_mm()
{
    return pwglfw.get_monitor_size_mm().to_f2();
}



void Auxwin::add_input_event(InputEvent event)
{
    events_input.push_back(event);
}

std::vector<InputEvent> Auxwin::get_events_input()
{
    auto return_vector = events_input;
    events_input.clear();
    return return_vector;
}


std::vector<WindowResizeEvent> Auxwin::get_events_window_resize()
{
    auto return_vector = events_resize;
    events_resize.clear();
    return return_vector;
}


void Auxwin::
framebuffer_callback(int _width, int _height)
{
    i2 frambuffer_size_i = i2{_width, _height};

    events_resize.emplace_back( frambuffer_size_i,
                                get_monitor_content_scale() );

    // update window structs
    pwglfw.update_dims_with_fb(frambuffer_size_i);
    // cursor_pos.window_dims = pwglfw.dims.window_size_f;
    reload_coordinate_constants();

    glViewport(0, 0, frambuffer_size_i.x, frambuffer_size_i.y);
}


void Auxwin::
mouse_button_callback(int button, int action, int mods)
{
    MouseButtonEvent mouse_button_event;

    if(action == GLFW_PRESS)
        mouse_button_event.action = ButtonAction::Press;
    else if (action == GLFW_RELEASE)
        mouse_button_event.action = ButtonAction::Release;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        mouse_button_event.button = MouseButton::Left;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        mouse_button_event.button = MouseButton::Right;
    else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        mouse_button_event.button = MouseButton::Middle;
    else if (button == GLFW_MOUSE_BUTTON_4)
        mouse_button_event.button = MouseButton::Aux1;
    else if (button == GLFW_MOUSE_BUTTON_5)
        mouse_button_event.button = MouseButton::Aux2;

    // CURSOR IMAGE
    if(mouse_button_event.button == MouseButton::Middle){
        if(mouse_button_event.action == ButtonAction::Press)
            set_cursor_state(Cursor::HAND);
        else if (mouse_button_event.action == ButtonAction::Release)
            set_cursor_state(Cursor::NORMAL);
    }
    if(mouse_button_event.button == MouseButton::Aux1)
        set_cursor_state(Cursor::CAPTURE);
    if(mouse_button_event.button == MouseButton::Aux2)
        set_cursor_state(Cursor::NORMAL);

    
    events_input.emplace_back(  modifiers_current,
                                EventType::MouseButton, 
                                mouse_button_event);
}

bool init_flag = false;
f2 input_prev;

void Auxwin::
cursor_position_callback(double xpos, double ypos)
{
    CursorPosition cursor_prev = cursor_pos;

    f2 input ((float) xpos, (float) ypos);
    if(!init_flag)
        input_prev = input;

    f2 input_delta = input - input_prev;


    cursor_pos.input        = input;
    cursor_pos.sane         = coord_transformer.i_s(input);
    cursor_pos.pixels       = coord_transformer.s_p(cursor_pos.sane);
    cursor_pos.normalized   = coord_transformer.s_n(cursor_pos.sane);
    cursor_pos.millimeters  = coord_transformer.s_m(cursor_pos.sane);


    MouseMoveEvent   mouse_movement (cursor_prev,
                                     cursor_pos);

    events_input.emplace_back(  modifiers_current,
                                EventType::MouseMove, 
                                mouse_movement);
}


void Auxwin::
scroll_callback(double xoffset, double yoffset)
{
    MouseScrollEvent   mouse_scroll ((float) yoffset);

    events_input.emplace_back(  modifiers_current,
                                EventType::MouseScroll, 
                                mouse_scroll);
}


void Auxwin::
key_callback(int key, int action, int mods)
{
    KeyStrokeEvent keystroke_event;

    keystroke_event.key = get_key_from_glfw_key(key);

    if(action == GLFW_PRESS)
        keystroke_event.action = ButtonAction::Press;
    else if(action == GLFW_RELEASE)
        keystroke_event.action = ButtonAction::Release;
    else if(action == GLFW_REPEAT)
        keystroke_event.action = ButtonAction::Hold;


    if(KeyModifiers::is_mod_key(keystroke_event.key))
    {
        if(keystroke_event.is_press())
            modifiers_current.press(keystroke_event.key);
        else if(keystroke_event.is_release())
            modifiers_current.release(keystroke_event.key);
        
        // println(KeyModifiers::to_str(modifiers_current));
    }
    
    try_close_keystroke(keystroke_event, modifiers_current);

    events_input.emplace_back(  modifiers_current,
                                EventType::Keystroke, 
                                keystroke_event     );
}

}


