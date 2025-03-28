
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "math/math.hh"

namespace window {


struct MouseState {
    bool    middle_down = false;
    f2  middle_prev_pos;
    f2  middle_delta_accum;

    float scroll_accumulator = 0.0f;

    f2 cursor_pos;
};

struct InputState {
    bool esc = false;

    bool p = false;

    bool f = false;
    bool b = false;
    bool r = false;
    bool l = false;
    bool u = false;
    bool d = false;

    bool s = false;
    bool m = false;

    bool shift = false;

    bool up = false;
    bool left = false;
    bool down = false;
    bool right = false;

    float scroll_delta = 0.0f;

    // Copies the authoritative state during creation
    MouseState mouse;
};

class Auxwin {
    public:
        GLFWwindow *glfw_window;
        bool open = true;

        Auxwin(int width, int height);
        
        void make_current();

        void new_frame();
        void end_frame();

        bool is_open();
        void close();
        void destroy();

        InputState get_input_state();
};


}