
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
    None, // Indicates end of event queue
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
    Hold,
};

struct MouseButtonEvent {
    MouseButton button;
    ButtonAction action;
};


struct MouseMoveEvent {
    f2 delta;

    f2 pos_px;
    f2 pos_norm;

    // MouseMoveEvent(f2 _delta) : delta { _delta } {};
    MouseMoveEvent(float _dx, float _dy, f2 _pos_px, f2 window_dims) : delta { f2(_dx, _dy) } {
        pos_px = _pos_px;
        pos_norm.x = pos_px.x / window_dims.x;
        pos_norm.y = pos_px.y / window_dims.y;
    };
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
    p,
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
    f2 size_f;

    WindowResizeEvent(i2 _size) : size { _size }, size_f { f2{ (float) _size.x, (float)_size.y}} {};
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
    InputEvent() : type { EventType::None} {};
};


struct MouseState {
    f2 cursor_pos;
    f2 window_dims;
    f2 cursor_pos_sane;

    // OLD
    f2 pos_prev;
    bool    middle_down = false;
    f2  middle_prev_pos;
    f2  middle_delta_accum;

    float scroll_accumulator = 0.0f;
};


class Auxwin {
    // Not working...
    // Maybe add using 'function' and 'bind'??
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

        // InputState get_input_state();
        std::queue<InputEvent> get_input_events();
        InputEvent get_input_event();
};


}