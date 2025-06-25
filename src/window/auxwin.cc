
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "print.hh"

#include "opengl/texture.hh"

#include "auxwin.hh"

namespace window {


Auxwin* current_auxwin;


void CursorPosition::print(){

    std::cout << std::endl;
    input.print("Input       ");
    sane.print("Sane        ");
    pixels.print("Pixel       ");
    normalized.print("Normalized  ");
    millimeters.print("Millimeters ");

}

/*
    GLFW C-Style callbacks as indirection layer
*/
void glfw_framebuffer_callback(GLFWwindow* _window, int _width, int _height) {
    current_auxwin->framebuffer_callback(_window, _width, _height);
}
void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
    current_auxwin->mouse_button_callback(window, button, action, mods);
}
void glfw_cursor_position_callback(GLFWwindow *window, double xpos, double ypos){
    current_auxwin->cursor_position_callback(window, xpos, ypos);
}

void glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    current_auxwin->scroll_callback(window, xoffset, yoffset);
}
void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    current_auxwin->key_callback(window, key, action, mods);
}





void Auxwin::init(int width, int height)
{
    current_auxwin = this;

    glfwInit();
    
    glfw_window_hints();

    glfw_create_window({width, height});

    glfw_create_cursors();

    opengl_init();
}



void Auxwin::reload_coordinate_constants_using_glfw()
{
    coords_input.monitor_size_px = get_monitor_size_px();
    coords_input.monitor_size_mm = get_monitor_size_mm();
    coords_input.content_scale = get_monitor_content_scale();
    coords_input.window_size_sc = get_window_fb_size_float() / coords_input.content_scale;

    coord_transformer.set_constants(coords_input);
}

void Auxwin::glfw_window_hints(){

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 8);

    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

}



void Auxwin::glfw_create_window(i2 window_size_i)
{

    glfw_window = glfwCreateWindow(window_size_i.x, window_size_i.y, "Auxwin", NULL, NULL);

    f2 content_scale = get_monitor_content_scale();

    f2 window_size_f = {(float)window_size_i.x, (float) window_size_i.y};

    window_fb_size = (window_size_f * content_scale).to_i2();

    cursor_pos.window_dims = window_size_f;

	// GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (glfw_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		// return -1;A
	}


    reload_coordinate_constants_using_glfw();


    glfwSetFramebufferSizeCallback( glfw_window, 
                                    glfw_framebuffer_callback      );

    glfwSetMouseButtonCallback(     glfw_window, 
                                    glfw_mouse_button_callback     );

    glfwSetCursorPosCallback(       glfw_window, 
                                    glfw_cursor_position_callback  );

    glfwSetScrollCallback(          glfw_window, 
                                    glfw_scroll_callback           );

    glfwSetKeyCallback(             glfw_window, 
                                    glfw_key_callback              );


    // TODO: THIS SHOULD NOT BE FORCED WHEN CREATING A NEW WINDOW!
    glfwMakeContextCurrent(glfw_window);

}
void Auxwin::opengl_init(){

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

    // OpenGL Fatures
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    opengl::textures_init();
}


void Auxwin::make_current()
{
    glfwMakeContextCurrent(glfw_window);
}


void Auxwin::
bind_window_framebuffer()
{
    glBindFramebuffer(  GL_FRAMEBUFFER, 
                        0               );

    glViewport( 0,
                0, 
                window_fb_size.x, 
                window_fb_size.y );
}


void Auxwin::
try_close()
{
    double current_time = glfwGetTime();
    double dt = current_time - time_of_last_close_key;

    if(dt < dt_to_close)
        close();
    else
        time_of_last_close_key = current_time;
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
    glfwSetWindowShouldClose(glfw_window, true);
    open = false;
}


bool Auxwin::
is_open(){
    return !glfwWindowShouldClose(glfw_window);
    // return open;
}


void Auxwin::new_frame(){

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    // glClear(GL_COLOR_BUFFER_BIT);
    // glEnable(GL_CULL_FACE);



    // // move all event in current queue to return array
    // while(other_input_events.size() > 0){
    //     return_arr.push_back(other_input_events.front());
    //     other_input_events.pop();
    // }


    // std::vector<InputEvent> return_arr = events_other;
    // events_other.clear();

    // return return_arr;

}
bool Auxwin::end_frame(){

    glBindVertexArray(0); 

    glfwSwapBuffers(glfw_window);
	glfwPollEvents();
    
    return is_open();
}


void Auxwin::destroy(){

    // TODO: SHOUDL NOT TERMINATE THE GLFW LIB!
    glfwTerminate();
}



void Auxwin::
glfw_create_cursors()
{
    // Set up cursors
    cursor.glfw_images.arrow        = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    cursor.glfw_images.cross_hair   = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    cursor.glfw_images.hand         = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    cursor.glfw_images.none         = glfwCreateStandardCursor(GLFW_CURSOR_HIDDEN);
}

void Auxwin::
set_cursor_state(Cursor::State new_state)
{
    // Prevent unnecesary cursor calls, which can be heavy
    if(cursor.state == new_state)
        return;

    cursor.state = new_state;

    switch (cursor.state)
    {
    case Cursor::NORMAL:
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursor(glfw_window, cursor.glfw_images.arrow);
        break;

    case Cursor::INVERTED:
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursor(glfw_window, cursor.glfw_images.cross_hair);
        break;

    case Cursor::HAND:
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursor(glfw_window, cursor.glfw_images.hand);
        break;

    case Cursor::CAPTURE:
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursor(glfw_window, cursor.glfw_images.none);
        // center_cursor_pos();
        // center_real_cursor();
        // events_input.clear();
        break;
    }
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
    return window_fb_size;
}

f2 Auxwin::
get_window_fb_size_float() const
{
    return {(float)window_fb_size.x, (float) window_fb_size.y};
}



f2 Auxwin::get_monitor_content_scale()
{
    f2 content_scale;

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorContentScale(primary_monitor, &content_scale.x, &content_scale.y);

    return content_scale;
}

f2 Auxwin::get_monitor_size_px()
{
    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);

    return f2{ (float)mode->width, (float)mode->height };
}

f2 Auxwin::get_monitor_size_mm()
{
    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();

    int width_mm, height_mm;
    glfwGetMonitorPhysicalSize(primary_monitor, &width_mm, &height_mm);

    return f2 { (float)width_mm, (float)height_mm };
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
framebuffer_callback(GLFWwindow* _window, int _width, int _height)
{
    // std::cout << "auxwin->framebuffer_callback" << std::endl;

    window_fb_size = {_width, _height};
    f2 window_fb_size_f = get_window_fb_size_float();

    f2 frambuffer_size_f = {(float) _width, (float) _height};
    i2 frambuffer_size_i = {_width, _height};

    f2 content_scale_f = get_monitor_content_scale();
    i2 content_scale_i = {(int)content_scale_f.x, (int)content_scale_f.y};

    f2 window_size_f = frambuffer_size_f / content_scale_f;
    i2 window_size_i = frambuffer_size_i / content_scale_i;

    cursor_pos.window_dims = window_size_f;
    
    reload_coordinate_constants_using_glfw();

    EventType event_type = EventType::WindowResize;

    WindowResizeEvent win_resize_event { 
        frambuffer_size_i,
        get_monitor_content_scale()
    };

    events_resize.emplace_back(win_resize_event);

    glViewport(0, 0, frambuffer_size_i.x, frambuffer_size_i.y);
}


void Auxwin::
mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
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
cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
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
scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    MouseScrollEvent   mouse_scroll ((float) yoffset);

    events_input.emplace_back(  modifiers_current,
                                EventType::MouseScroll, 
                                mouse_scroll);
}


void Auxwin::
key_callback(GLFWwindow *window, int key, int action, int mods)
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