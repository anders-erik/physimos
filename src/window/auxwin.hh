
#pragma once

#include <queue>

#include "math/vecmat.hh"


#include "auxevent.hh"

// To not require the inclusion of the glfw header
struct GLFWwindow;
struct GLFWcursor;


namespace window {


struct MouseState {
    f2 pos_raw;
    
    // Positive y is measured from bottom of window to top
    f2 pos;
    // Positive y is measured from bottom of window to top
    f2 pos_prev;
    // Last mouse movement delta
    f2 pos_delta;

    f2 window_dims; // TODO: move to auxwin when callbacks have been moved into class
};



class Auxwin {

    private:

        void glfw_window_hints();
        void glfw_create_window(int width, int height);
        void opengl_init();

        MouseState mouse_state;

        std::queue<InputEvent> input_events;

        GLFWcursor* default_cursor;
        GLFWcursor* pan_cursor;

    public:
        GLFWwindow *glfw_window;
        bool open = true;


        Auxwin() {};

        void init(int width, int height);
        
        void make_current();

        void new_frame();
        void end_frame();

        bool is_open();
        void close();
        void destroy();

        std::queue<InputEvent> get_input_events();
        InputEvent get_input_event();
        /** Create an artificial event. */
        void add_input_event(InputEvent event);


        void framebuffer_callback(GLFWwindow* _window, int _width, int _height);
        void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
        void mouse_position_callback(GLFWwindow *window, double xpos, double ypos);
        void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
        void key_callback(GLFWwindow *window, int key, int action, int mods);
};



}