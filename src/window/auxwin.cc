
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





Auxwin::Auxwin(f2 new_window_size){
    init(new_window_size);
}

void Auxwin::init(int width, int height){
    current_window_size_f = { (float) width, (float) height};
    current_window_size_i = { width, height};
    init();
}

void Auxwin::init(i2 new_window_size){
    current_window_size_f = { (float) new_window_size.x, (float) new_window_size.y};
    current_window_size_i = new_window_size;
    init();
}
void Auxwin::init(f2 new_window_size){
    current_window_size_f = new_window_size;
    current_window_size_i = { (int) new_window_size.x, (int) new_window_size.y};
    init();
}
void Auxwin::init(){
    current_auxwin = this;

    glfwInit();
    
    glfw_window_hints();
    glfw_create_window();
    opengl_init();
}


void Auxwin::reload_coordinate_constants_using_glfw()
{
    coords_input.monitor_size_px = get_monitor_size_px();
    coords_input.monitor_size_mm = get_monitor_size_mm();
    coords_input.content_scale = get_monitor_content_scale();
    coords_input.window_size_sc = current_window_size_f;

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
void Auxwin::glfw_create_window(){

    cursor.window_dims = current_window_size_f;

    glfw_window = glfwCreateWindow(current_window_size_i.x, current_window_size_i.y, "Auxwin", NULL, NULL);


	// GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (glfw_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		// return -1;A
	}

    // Set up cursors
    pan_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    default_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);


    reload_coordinate_constants_using_glfw();


    glfwSetFramebufferSizeCallback(glfw_window, glfw_framebuffer_callback);
    glfwSetMouseButtonCallback(glfw_window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(glfw_window, glfw_cursor_position_callback);
    glfwSetScrollCallback(glfw_window, glfw_scroll_callback);
    glfwSetKeyCallback(glfw_window, glfw_key_callback);


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
void Auxwin::bind_window_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, current_window_size_i.x, current_window_size_i.y);
    
}
void Auxwin::try_close()
{
    double current_time = glfwGetTime();
    double dt = current_time - time_of_last_close_key;

    if(dt < dt_to_close)
        close();
    else
        time_of_last_close_key = current_time;
}
void Auxwin::close(){
    glfwSetWindowShouldClose(glfw_window, true);
    open = false;
}
bool Auxwin::is_open(){
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
    if(event.is_window_resize())
        events_resize.push_back(event);
    else
        events_input.push_back(event);
}

std::vector<InputEvent> Auxwin::get_events_input()
{
    auto return_vector = events_input;
    events_input.clear();
    return return_vector;
}


std::vector<InputEvent> Auxwin::get_events_window_resize()
{
    auto return_vector = events_resize;
    events_resize.clear();
    return return_vector;
}


void Auxwin::
framebuffer_callback(GLFWwindow* _window, int _width, int _height)
{
    // std::cout << "auxwin->framebuffer_callback" << std::endl;

    current_window_size_f = {(float) _width, (float) _height};
    current_window_size_i = {_width, _height};
    cursor.window_dims = current_window_size_f;
    
    reload_coordinate_constants_using_glfw();

    EventType event_type = EventType::WindowResize;
    // i2 size (_width, _height);
    WindowResizeEvent win_resize_event { 
        i2(_width, _height),
        get_monitor_content_scale()
    };
    events_resize.emplace_back(glfw_window, event_type, win_resize_event, modifiers_current);

    cursor.window_dims.x = (float) _width;
    cursor.window_dims.y = (float) _height;

    glViewport(0, 0, _width, _height);
}


void Auxwin::
mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
    
    EventType event_type = EventType::MouseButton;
    MouseButtonEvent mouse_button_event;

    if(action == GLFW_PRESS)
        mouse_button_event.action = ButtonAction::Press;
    else if (action == GLFW_RELEASE)
        mouse_button_event.action = ButtonAction::Release;
    
    if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        mouse_button_event.button = MouseButton::Middle;
    else if (button == GLFW_MOUSE_BUTTON_LEFT)
        mouse_button_event.button = MouseButton::Left;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        mouse_button_event.button = MouseButton::Right;

    // CURSOR IMAGE
    if(mouse_button_event.button == MouseButton::Middle){
        if(mouse_button_event.action == ButtonAction::Press)
            glfwSetCursor(window, pan_cursor);
        else if (mouse_button_event.action == ButtonAction::Release)
            glfwSetCursor(window, default_cursor);
    }

    
    events_input.emplace_back(window, event_type, mouse_button_event, modifiers_current);
}


void Auxwin::
cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{

    CursorPosition cursor_prev = cursor;

    f2 input ((float) xpos, (float) ypos);

    // Update current cursor
    cursor.input = input;
    cursor.sane = coord_transformer.i_s(input);
    cursor.pixels = coord_transformer.s_p(cursor.sane);
    cursor.normalized = coord_transformer.s_n(cursor.sane);
    cursor.millimeters = coord_transformer.s_m(cursor.sane);


    EventType       event_type = EventType::MouseMove;
    MouseMoveEvent   mouse_movement (
                                        cursor_prev,
                                        cursor
                                    );

    events_input.emplace_back(window, event_type, mouse_movement, modifiers_current);

}


void Auxwin::
scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    // f2 delta ((float)xpos, (float)ypos);

    EventType       event_type = EventType::MouseScroll;
    MouseScrollEvent   mouse_scroll ((float) yoffset);

    events_input.emplace_back(window, event_type, mouse_scroll, modifiers_current);
}


void Auxwin::
key_callback(GLFWwindow *window, int key, int action, int mods)
{

    KeyStrokeEvent keystroke_event;

    EventType event_type = EventType::Keystroke;

    // KEY
    keystroke_event.key = get_key_from_glfw_key(key);


    // BUTTON ACTION
    if(action == GLFW_PRESS)
        keystroke_event.action = ButtonAction::Press;
    else if(action == GLFW_RELEASE)
        keystroke_event.action = ButtonAction::Release;
    else if(action == GLFW_REPEAT)
        keystroke_event.action = ButtonAction::Hold;

    // MODIFIER KEY
    switch (mods)
    {
    case 0: modifier_current_state = KeyModifierState::none            ;   break;
    case 1: modifier_current_state = KeyModifierState::shift           ;   break;
    case 2: modifier_current_state = KeyModifierState::ctl             ;   break;
    case 3: modifier_current_state = KeyModifierState::ctl_shift       ;   break;
    case 4: modifier_current_state = KeyModifierState::alt             ;   break;
    case 5: modifier_current_state = KeyModifierState::alt_shift       ;   break;
    case 6: modifier_current_state = KeyModifierState::alt_ctrl        ;   break;
    case 7: modifier_current_state = KeyModifierState::alt_ctl_shift   ;   break;
    
    default: modifier_current_state = KeyModifierState::none           ;   break;
    }
    
    switch (key)
    {
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
        if(action == GLFW_PRESS)
            modifiers_current.set_shift();
        if(action == GLFW_RELEASE)
            modifiers_current.unset_shift();
        break;
    
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        if(action == GLFW_PRESS)
            modifiers_current.set_ctrl();
        if(action == GLFW_RELEASE)
            modifiers_current.unset_ctrl();
        break;

    case GLFW_KEY_LEFT_ALT:
    case GLFW_KEY_RIGHT_ALT:
        if(action == GLFW_PRESS)
            modifiers_current.set_alt();
        if(action == GLFW_RELEASE)
            modifiers_current.unset_alt();
        break;
    
    default:
        break;
    }
    

    keystroke_event.modifier = modifier_current_state;


    // CHECK CLOSE KEY
    if(keystroke_event.is(close_key) && keystroke_event.is(ButtonAction::Press))
        try_close();


    events_input.emplace_back(window, event_type, keystroke_event, modifiers_current);
}

}