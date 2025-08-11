
#include <window/glfw.hh>

#include "window/auxwin.hh"


void PWGLFW::
glfw_framebuffer_callback(GLFWwindow* _window, int _width, int _height)
{
    get_current_auxwin()->framebuffer_callback(_width, _height);
}


void PWGLFW::
glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    get_current_auxwin()->mouse_button_callback(button, action, mods);
}


void PWGLFW::
glfw_cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    get_current_auxwin()->cursor_position_callback(xpos, ypos);
}


void PWGLFW::
glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    get_current_auxwin()->scroll_callback(xoffset, yoffset);
}


void PWGLFW::
glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    get_current_auxwin()->key_callback(key, action, mods);
}
