
#pragma once

#include "math/vecmat.hh"

#include "window/auxkey.hh"


struct GLFWwindow; // bypass glfw inclusion

namespace window {


struct CursorPosition {
    f2 input;
    f2 sane;
    f2 normalized;
    f2 pixels;
    f2 millimeters;

    void print();

    f2 window_dims; // TODO: move to auxwin when callbacks have been moved into class
};


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

    CursorPosition cursor;

    f2 delta;

    MouseMoveEvent(CursorPosition _cursor_pos, f2 _delta) : cursor {_cursor_pos}, delta { _delta } {};
    MouseMoveEvent() = default;
};

struct MouseScrollEvent {
    float delta;

    MouseScrollEvent(float _delta) : delta { _delta } {};
    MouseScrollEvent() : delta { 0.0f } {};
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
    GLFWwindow *glfw_window;
    EventType type;
    
    MouseButtonEvent mouse_button;
    MouseMoveEvent mouse_movement;
    MouseScrollEvent mouse_scroll;
    KeyStrokeEvent key_stroke;
    WindowResizeEvent window_resize;

    InputEvent(GLFWwindow* _glfw_window, EventType _type, MouseButtonEvent _mouse_button) :  glfw_window {_glfw_window}, type {_type}, mouse_button {_mouse_button}    {};
    InputEvent(GLFWwindow* _glfw_window, EventType _type, MouseMoveEvent _mouse_movement) :  glfw_window {_glfw_window}, type {_type}, mouse_movement {_mouse_movement}{};
    InputEvent(GLFWwindow* _glfw_window, EventType _type, MouseScrollEvent _mouse_scroll) :  glfw_window {_glfw_window}, type {_type}, mouse_scroll { _mouse_scroll}   {};
    InputEvent(GLFWwindow* _glfw_window, EventType _type, KeyStrokeEvent _key_stroke)     :  glfw_window {_glfw_window}, type {_type}, key_stroke { _key_stroke}       {};
    InputEvent(GLFWwindow* _glfw_window, EventType _type, WindowResizeEvent _window_resize): glfw_window {_glfw_window}, type {_type}, window_resize { _window_resize} {};
    InputEvent() : type { EventType::None} {};
};




}

