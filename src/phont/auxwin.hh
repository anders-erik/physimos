
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct InputState {
    bool esc = false;
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

        InputState handle_input();
};

