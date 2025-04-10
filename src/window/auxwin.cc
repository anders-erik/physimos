
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "auxwin.hh"

namespace window {


MouseState mouse_state;

std::queue<InputEvent> input_events;

GLFWcursor* default_cursor;
GLFWcursor* pan_cursor;


void framebuffer_callback(GLFWwindow* _window, int _width, int _height) {

    EventType event_type = EventType::WindowResize;
    // i2 size (_width, _height);
    WindowResizeEvent win_resize_event (i2(_width, _height)) ;
    input_events.emplace(event_type, win_resize_event);

    mouse_state.window_dims.x = (float) _width;
    mouse_state.window_dims.y = (float) _height;

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
            // mouse_state.middle_down = true;
            
            glfwSetCursor(window, pan_cursor);

            mouse_button_event.action = ButtonAction::Press;
        }
        else if (action == GLFW_RELEASE){
            // std::cout << "moddle rel" << std::endl;
            // mouse_state.middle_down = false;
            glfwSetCursor(window, default_cursor);

            mouse_button_event.action = ButtonAction::Release;
        }

    }
    
    // InputEvent input_event (event_type, mouse_button_event);
    // input_events.push(input_event);
    input_events.emplace(InputEvent (event_type, mouse_button_event));
}

void mouse_position_callback(GLFWwindow *window, double xpos, double ypos){

    // store raw for debugging
    mouse_state.pos_raw = {(float)xpos, (float)ypos};

    // Use my own coordinate system
    mouse_state.pos = { mouse_state.pos_raw.x, mouse_state.window_dims.y - mouse_state.pos_raw.y};

    mouse_state.pos_delta.x = mouse_state.pos.x - mouse_state.pos_prev.x;
    mouse_state.pos_delta.y = mouse_state.pos.y - mouse_state.pos_prev.y;

    mouse_state.pos_prev = mouse_state.pos;


    EventType       event_type = EventType::MouseMove;
    MouseMoveEvent   mouse_movement (
                                        mouse_state.pos_delta.x, 
                                        mouse_state.pos_delta.y, 
                                        mouse_state.pos, 
                                        mouse_state.window_dims
                                    );

    input_events.emplace(event_type, mouse_movement);

}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){

    // f2 delta ((float)xpos, (float)ypos);

    EventType       event_type = EventType::MouseScroll;
    MouseScrollEvent   mouse_scroll ((float) yoffset);

    input_events.emplace(event_type, mouse_scroll);

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


    input_events.emplace(event_type, keystroke_event);
}



Auxwin::Auxwin(int width, int height){
    glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 8);
    

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

    mouse_state.window_dims.x = (float) width;
    mouse_state.window_dims.y = (float) height;

    // Set up cursors
    pan_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

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


std::queue<InputEvent> Auxwin::get_input_events(){
    std::queue<InputEvent> tmp_queue = input_events;

    while(input_events.size() > 0)
        input_events.pop();

    return tmp_queue;
}

InputEvent Auxwin::get_input_event(){
    InputEvent input_event;

    if(input_events.size() == 0){
        input_event.type = EventType::None;
    }
    else {
        input_event = input_events.front();
        input_events.pop();
    }

    return input_event;
}

}