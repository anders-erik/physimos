
#pragma once

#include <queue>

#include "math/vecmat.hh"

struct GLFWwindow;

namespace window {


void framebuffer_callback(GLFWwindow *window, int width, int height);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void mouse_position_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);



enum class EventType {
    MouseButton,
    MouseMove,
    MouseScroll,
    Keystroke,
    WindowResize,
};

enum class MouseButton {
    Left,
    Right,
    Middle,
    Aux1,
    Aux2,
};

enum ButtonAction {
    Press,
    Release,
    Hold, // ??
};

struct MouseButtonEvent {
    MouseButton button;
    ButtonAction action;
};


struct MouseMoveEvent {
    f2 delta;

    MouseMoveEvent(f2 _delta) : delta { _delta } {};
    MouseMoveEvent(float _dx, float _dy) : delta { f2(_dx, _dy) } {};
    MouseMoveEvent() = default;
};

struct MouseScrollEvent {
    float delta;

    MouseScrollEvent(float _delta) : delta { _delta } {};
    MouseScrollEvent() : delta { 0.0f } {};
};

enum class Key {
    None,
    Esc,
    p
};

enum class KeyModifier :int {
    alt_ctl_shift   = 7,
    alt_ctrl        = 6,
    alt_shift       = 5,
    alt             = 4,
    ctl_shift       = 3,
    ctl             = 2,
    shift           = 1,
    none            = 0,
};

struct KeyStrokeEvent {
    Key key = Key::None;
    KeyModifier modifier;
    ButtonAction action;

    KeyStrokeEvent(Key _key, KeyModifier _modifier, ButtonAction _action) : key         {_key},
                                                                            modifier    {_modifier},
                                                                            action      {_action} {};
    KeyStrokeEvent() = default;
};

struct WindowResizeEvent {
    i2 size;

    WindowResizeEvent(i2 _size) : size { _size } {};
    WindowResizeEvent() = default;
};

struct InputEvent {
    EventType type;
    
    MouseButtonEvent mouse_button;
    MouseMoveEvent mouse_movement;
    MouseScrollEvent mouse_scroll;
    KeyStrokeEvent key_stroke;
    WindowResizeEvent window_resize;

    InputEvent(EventType _type, MouseButtonEvent _mouse_button) : type {_type}, mouse_button {_mouse_button}    {};
    InputEvent(EventType _type, MouseMoveEvent _mouse_movement) : type {_type}, mouse_movement {_mouse_movement}{};
    InputEvent(EventType _type, MouseScrollEvent _mouse_scroll) : type {_type}, mouse_scroll { _mouse_scroll}   {};
    InputEvent(EventType _type, KeyStrokeEvent _key_stroke)     : type {_type}, key_stroke { _key_stroke}       {};
    InputEvent(EventType _type, WindowResizeEvent _window_resize):type {_type}, window_resize { _window_resize} {};
};


struct MouseState {
    f2 pos_prev;
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

    MouseButtonEvent mouse_button;
};


class Auxwin {
    // Not working...
    // void framebuffer_callback(GLFWwindow *window, int width, int height);
    // void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    // void mouse_position_callback(GLFWwindow *window, double xpos, double ypos);
    // void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    // void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


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
        std::queue<InputEvent> get_input_events();
};


}