
#include <iostream>
#include <set>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "auxwin.hh"

namespace window {

InputState input_state;

MouseState mouse_state;
GLFWcursor* default_cursor;
GLFWcursor* pan_cursor;

std::queue<InputEvent> input_events;
std::set<Key>    keys_pressed;

void framebuffer_callback(GLFWwindow* _window, int _width, int _height) {

    EventType event_type = EventType::WindowResize;
    // i2 size (_width, _height);
    WindowResizeEvent win_resize_event (i2(_width, _height)) ;
    input_events.emplace(event_type, win_resize_event);

    glViewport(0, 0, _width, _height);
    // scene_set_viewport_dims(_width, _height);
}


void mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
    EventType event_type = EventType::MouseButton;
    MouseButtonEvent mouse_button_event;

    // InputEvent& new_event = event_queue.emplace(EventType::MouseButton, EventMouseButton:: );


    // std::cout << "mouse butn" << std::endl;
    if(button == GLFW_MOUSE_BUTTON_MIDDLE){

        mouse_button_event.button = MouseButton::Middle;

        if(action == GLFW_PRESS){
            // std::cout << "moddle press" << std::endl;
            mouse_state.middle_down = true;
            
            glfwSetCursor(window, pan_cursor);

            mouse_button_event.action = ButtonAction::Press;
        }
        else if (action == GLFW_RELEASE){
            // std::cout << "moddle rel" << std::endl;
            mouse_state.middle_down = false;
            glfwSetCursor(window, default_cursor);

            mouse_button_event.action = ButtonAction::Release;
        }

        mouse_state.middle_delta_accum.x = 0.0;
        mouse_state.middle_delta_accum.y = 0.0;
        mouse_state.middle_prev_pos.x = mouse_state.cursor_pos.x;
        mouse_state.middle_prev_pos.y = mouse_state.cursor_pos.y;
    }
    
    // InputEvent input_event (event_type, mouse_button_event);
    // input_events.push(input_event);
    input_events.emplace(InputEvent (event_type, mouse_button_event));
}

void mouse_position_callback(GLFWwindow *window, double xpos, double ypos){

    // std::cout << "mouse mouse" << std::endl;
    // std::cout << "accum x = " << mouse_state.middle_delta_accum.x << std::endl;
    // std::cout << "accum y = " << mouse_state.middle_delta_accum.y << std::endl;

    // Update Delta
    f2 pos ((float)xpos, (float)ypos);

    float dx =    pos.x - mouse_state.pos_prev.x;
    float dy = - (pos.y - mouse_state.pos_prev.y); // Positive is up

    mouse_state.pos_prev = pos;


    EventType       event_type = EventType::MouseMove;
    MouseMoveEvent   mouse_movement (dx, dy);

    input_events.emplace(event_type, mouse_movement);


    // OLD
    if(mouse_state.middle_down){
        mouse_state.middle_delta_accum.x += xpos - mouse_state.middle_prev_pos.x;
        mouse_state.middle_delta_accum.y += mouse_state.middle_prev_pos.y - ypos;

        mouse_state.middle_prev_pos.x = xpos;
        mouse_state.middle_prev_pos.y = ypos;
    
    }

    mouse_state.cursor_pos.x = xpos;
    mouse_state.cursor_pos.y = ypos;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){

    // f2 delta ((float)xpos, (float)ypos);

    EventType       event_type = EventType::MouseScroll;
    MouseScrollEvent   mouse_scroll ((float) yoffset);

    input_events.emplace(event_type, mouse_scroll);


    mouse_state.scroll_accumulator += (float) yoffset;
    // std::cout << "mouse_state.scroll_accumulator = " << mouse_state.scroll_accumulator << std::endl;
}

Key get_key_from_glfw_key(int _glfw_key){
    Key key;
    switch (_glfw_key){

    case GLFW_KEY_ESCAPE:   key = Key::Esc;       break;
    case GLFW_KEY_P:        key = Key::p;       break;
    
    default:
        break;
    }

    return key;

}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){

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
    switch (mods){

    case 0: keystroke_event.modifier = KeyModifier::none            ;   break;
    case 1: keystroke_event.modifier = KeyModifier::shift           ;   break;
    case 2: keystroke_event.modifier = KeyModifier::ctl             ;   break;
    case 3: keystroke_event.modifier = KeyModifier::ctl_shift       ;   break;
    case 4: keystroke_event.modifier = KeyModifier::alt             ;   break;
    case 5: keystroke_event.modifier = KeyModifier::alt_shift       ;   break;
    case 6: keystroke_event.modifier = KeyModifier::alt_ctrl        ;   break;
    case 7: keystroke_event.modifier = KeyModifier::alt_ctl_shift   ;   break;
    
    default: keystroke_event.modifier = KeyModifier::none           ;   break;
    }


    // Originally used a set to detect hold. Turns out im stupid and never realized that glfw had 'GLFW_REPEAT'

    // ButtonAction registered_action = action == GLFW_PRESS ? ButtonAction::Press : ButtonAction::Release;
    // if(registered_action == GLFW_PRESS){ 

    //     if(keys_pressed.count(keystroke.key) > 0){
    //         keystroke.action = ButtonAction::Hold;
    //     }
    //     else {
    //         keystroke_event.action = ButtonAction::Press;
    //         keys_pressed.insert(keystroke.key);
    //     }

    // }
    // else if(action == GLFW_RELEASE){
    //     keystroke_event.action = ButtonAction::Release;
    //     keys_pressed.erase(keystroke_event.key);
    // }
   

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        input_state.up = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        input_state.left = true;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        input_state.down = true;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        input_state.right = true;
    

    input_events.emplace(event_type, keystroke_event);
}

// void framebuffer_size_callback_2(GLFWwindow *window, int width, int height){
// 	glViewport(0, 0, width, height);
// }

Auxwin::Auxwin(int width, int height){
    glfwInit();


	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 1);
    

    glfw_window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);

	// GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (glfw_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		// return -1;A
	}

    // glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback_2);

    // TODO: THIS SHOULD NOT BE FORCED WHEN CREATING A NEW WINDOW!
    glfwMakeContextCurrent(glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}


    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
    glfwSetCursorPosCallback(glfw_window, mouse_position_callback);
    glfwSetScrollCallback(glfw_window, scroll_callback);
    glfwSetKeyCallback(glfw_window, key_callback);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Auxwin::make_current(){
    glfwMakeContextCurrent(glfw_window);
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
}
void Auxwin::end_frame(){
    glBindVertexArray(0); 

    glfwSwapBuffers(glfw_window);
	glfwPollEvents();
}


void Auxwin::destroy(){

    // TODO: SHOUDL NOT TERMINATE THE GLFW LIB!
    glfwTerminate();
}



InputState Auxwin::get_input_state(){
    // InputState input_state;

    // if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //     input_state.esc = true;
	// 	// auxwin.close();
    
    // return input_state;


    InputState _input_state = input_state;
    // Reset
    input_state = InputState();

    // Mouse
    _input_state.mouse = mouse_state;
    // reset
    mouse_state.middle_delta_accum.x = 0.0;
    mouse_state.middle_delta_accum.y = 0.0;


    _input_state.scroll_delta = mouse_state.scroll_accumulator;
    mouse_state.scroll_accumulator = 0.0f;

    if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        _input_state.esc = true;


    if (glfwGetKey(glfw_window, GLFW_KEY_P) == GLFW_PRESS)
        _input_state.p = true;


    if (glfwGetKey(glfw_window, GLFW_KEY_F) == GLFW_PRESS)
        _input_state.f = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_B) == GLFW_PRESS)
        _input_state.b = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_R) == GLFW_PRESS)
        _input_state.r = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_L) == GLFW_PRESS)
        _input_state.l = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_U) == GLFW_PRESS)
        _input_state.u = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
        _input_state.d = true;

    if(glfwGetKey(glfw_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(glfw_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        _input_state.shift = true;

    if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
        _input_state.s = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_M) == GLFW_PRESS)
        _input_state.m = true;


    if (glfwGetKey(glfw_window, GLFW_KEY_UP) == GLFW_PRESS)
        input_state.up = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        input_state.left = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        input_state.down = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        input_state.right = true;


    return _input_state;
}

std::queue<InputEvent> Auxwin::get_input_events(){
    std::queue<InputEvent> tmp_queue = input_events;

    while(input_events.size() > 0)
        input_events.pop();

    return tmp_queue;
}

}