
#pragma once

#include "math/vecmat.hh"

#include "window/auxkey.hh"


namespace window {


struct CursorPosition {
    f2 input;
    f2 sane;
    f2 normalized;
    f2 pixels;
    f2 millimeters;

    CursorPosition operator-(const CursorPosition& rhs){
        CursorPosition difference;
        difference.input       = input       - rhs.input;
        difference.sane        = sane        - rhs.sane;
        difference.normalized  = normalized  - rhs.normalized;
        difference.pixels      = pixels      - rhs.pixels;
        difference.millimeters = millimeters - rhs.millimeters;
        return difference;
    }

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

struct MouseButtonEvent 
{
    MouseButton button;
    ButtonAction action;

    bool is_left_down()
    {
        return button == MouseButton::Left && action == ButtonAction::Press;
    }
    bool is_left_up()
    {
        return button == MouseButton::Left && action == ButtonAction::Release;
    }

    bool is_middle_down()
    {
        return button == MouseButton::Middle && action == ButtonAction::Press;
    }
    bool is_middle_up()
    {
        return button == MouseButton::Middle && action == ButtonAction::Release;
    }
};


struct MouseMoveEvent {

    CursorPosition cursor_prev;
    CursorPosition cursor_new;
    CursorPosition delta;

    MouseMoveEvent() = default;
    MouseMoveEvent(CursorPosition& _cursor_prev, CursorPosition& _cursor_pos) 
        :   cursor_prev {_cursor_prev}, 
            cursor_new {_cursor_pos},
            delta {_cursor_pos - _cursor_prev} {};
          
};

struct MouseScrollEvent {
    float delta;

    MouseScrollEvent(float _delta) : delta { _delta } {};
    MouseScrollEvent() : delta { 0.0f } {};
};



struct KeyStrokeEvent {
    Key key = Key::None;
    ButtonAction action;

    KeyStrokeEvent() = default;
    KeyStrokeEvent(Key _key, ButtonAction _action) 
        :   key         {_key},
            action      {_action} 
    {
    };

    bool is_esc()       {return key == Key::Esc;                    }
    bool is_press()     {return action == ButtonAction::Press;      }
    bool is_not_press() {return action != ButtonAction::Press;      }
    bool is_release()   {return action == ButtonAction::Release;    }

    bool is(Key key_to_check)           {return key == key_to_check;    }
    bool is(ButtonAction button_action) {return action == button_action;}
};

struct WindowResizeEvent {
    i2 size_i;
    f2 size_f;
    f2 content_scale; // This simply queries the current content scale. It does NOT respond the content scale changes

    WindowResizeEvent() = default;
    WindowResizeEvent(i2 _size, f2 content_scale) 
        :   size_i { _size }, 
            size_f { f2{ (float) _size.x, (float)_size.y}},
            content_scale {content_scale} {};
};



struct InputEvent 
{
    KeyModifiers modifiers;
    EventType type = EventType::None;
    
    // TODO: turn into variant
    MouseButtonEvent mouse_button;
    MouseMoveEvent mouse_movement;
    MouseScrollEvent mouse_scroll;
    KeyStrokeEvent keystroke;


    InputEvent( KeyModifiers        modifier_state,
                EventType           _type, 
                MouseButtonEvent    _mouse_button) 
        :   modifiers       {modifier_state},
            type            {_type}, 
            mouse_button    {_mouse_button}
    {
    };

    InputEvent( KeyModifiers        modifier_state,
                EventType   _type, 
                MouseMoveEvent _mouse_movement) 
        :   modifiers       {modifier_state},
            type            {_type}, 
            mouse_movement  {_mouse_movement}
    {
    };

    InputEvent( KeyModifiers        modifier_state,
                EventType _type, 
                MouseScrollEvent _mouse_scroll) 
        :   modifiers       {modifier_state},
            type            {_type}, 
            mouse_scroll    { _mouse_scroll}
    {
    };

    InputEvent( KeyModifiers        modifier_state,
                EventType _type, 
                KeyStrokeEvent _key_stroke)
        :   modifiers       {modifier_state},
            type            {_type}, 
            keystroke       { _key_stroke}
    {
    };


    /** Is any of the mouse events (move, scroll, click). */
    bool is_mouse()
    {
        return  (type == EventType::MouseButton)    ||
                (type == EventType::MouseMove)      ||
                (type == EventType::MouseScroll);
    }
    bool is_mouse_button()  {return type == EventType::MouseButton; }
    bool is_mouse_movement(){return type == EventType::MouseMove  ; }
    bool is_mouse_scroll()  {return type == EventType::MouseScroll; }
    bool is_keystroke()     {return type == EventType::Keystroke  ; }
};



}






/** 
	InputEvent-response intended to be returned by an input event handler.
    Default contructor should always contain action to reset all states tracked by reciever of response. 
*/
struct InputResponse
{
    typedef unsigned short ActionBits;

    static const ActionBits MOUSE_RELEASE       = 0x0001;
    static const ActionBits MOUSE_GRAB          = 0x0002;
    static const ActionBits MOUSE_PAN           = 0x0004;

    static const ActionBits KEYBOARD_RELEASE    = 0x0010;
    static const ActionBits KEYBOARD_GRAB       = 0x0020;

    static const ActionBits BOTH_RELEASED       = MOUSE_RELEASE | KEYBOARD_RELEASE;


	ActionBits action;

    InputResponse()                         : action {BOTH_RELEASED}    {}
    InputResponse(ActionBits action_bits)   : action {action_bits}      {}
    
    bool is(ActionBits bits_to_test) 
    {   
        return (bits_to_test & action) == bits_to_test;
    }

    bool is_mouse_grab()            {   return  is(MOUSE_GRAB);                     }
    bool is_mouse_pan()             {   return  is(MOUSE_PAN);                      }
    bool is_mouse_release()         {   return  is(MOUSE_RELEASE);                  }
    bool is_mouse_non_release()     {   return !is_mouse_release();                 }

	bool is_keyboard_grab()         {   return  is(KEYBOARD_GRAB);                  }
    bool is_keyboard_release()      {   return  is(KEYBOARD_RELEASE);               }

    bool is_release_both()          {   return  is(BOTH_RELEASED);                  }
};

