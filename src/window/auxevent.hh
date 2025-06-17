
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

    CursorPosition operator-(const CursorPosition& rhs){
        CursorPosition difference;
        difference.input = this->input - rhs.input;
        difference.sane = this->sane - rhs.sane;
        difference.normalized = this->normalized - rhs.normalized;
        difference.pixels = this->pixels - rhs.pixels;
        difference.millimeters = this->millimeters - rhs.millimeters;
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

struct MouseButtonEvent {
    MouseButton button;
    ButtonAction action;

    bool is_left_down(){
        if(button == MouseButton::Left && action == ButtonAction::Press)
            return true;
        return false;
    }
    bool is_left_up(){
        if(button == MouseButton::Left && action == ButtonAction::Release)
            return true;
        return false;
    }

    bool is_middle_down(){
        if(button == MouseButton::Middle && action == ButtonAction::Press)
            return true;
        return false;
    }
    bool is_middle_up(){
        if(button == MouseButton::Middle && action == ButtonAction::Release)
            return true;
        return false;
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
    KeyModifierState modifier;
    ButtonAction action;

    KeyStrokeEvent(Key _key, KeyModifierState _modifier, ButtonAction _action) 
        :   key         {_key},
            modifier    {_modifier},
            action      {_action} 
    {
    };
    KeyStrokeEvent() = default;

    bool is(Key key) {
        return this->key == key ? true : false;
    }
    bool is(ButtonAction button_action) {
        return this->action == button_action ? true : false;
    }
    bool has(KeyModifierState key_modifier) {
        return this->modifier == key_modifier ? true : false;
    }
};

struct WindowResizeEvent {
    i2 size;
    f2 size_f;
    f2 content_scale; // This simply queries the current content scale. It does NOT respond the content scale changes

    WindowResizeEvent(i2 _size, f2 content_scale) 
        :   size { _size }, 
            size_f { f2{ (float) _size.x, (float)_size.y}},
            content_scale {content_scale} {};
    WindowResizeEvent() = default;
};



struct InputEvent 
{
    GLFWwindow *glfw_window;
    // Cursor position at the moment of the event. 
    // For mouse movement events this would be the location of the cursor *before* the move took place.
    CursorPosition      cursor_pos; 

    EventType type;
    
    MouseButtonEvent mouse_button;
    MouseMoveEvent mouse_movement;
    MouseScrollEvent mouse_scroll;
    KeyStrokeEvent keystroke;

    KeyModifiers modifiers;


    InputEvent() : type { EventType::None} {};

    InputEvent( GLFWwindow*         _glfw_window, 
                CursorPosition      cursor_pos, 
                EventType           _type, 
                MouseButtonEvent    _mouse_button, 
                KeyModifiers        modifier_state) 
        :   glfw_window     {_glfw_window}, 
            cursor_pos      {cursor_pos}, 
            type            {_type}, 
            mouse_button    {_mouse_button}, 
            modifiers       {modifier_state} 
    {
    };

    InputEvent( GLFWwindow* _glfw_window,
                CursorPosition      cursor_pos,
                EventType   _type, 
                MouseMoveEvent _mouse_movement, 
                KeyModifiers modifier_state) 
        :   glfw_window     {_glfw_window}, 
            cursor_pos      {cursor_pos}, 
            type            {_type}, 
            mouse_movement  {_mouse_movement}, 
            modifiers       {modifier_state} 
    {
    };

    InputEvent( GLFWwindow* _glfw_window, 
                CursorPosition      cursor_pos,
                EventType _type, 
                MouseScrollEvent _mouse_scroll,
                KeyModifiers modifier_state) 
        :   glfw_window     {_glfw_window}, 
            cursor_pos      {cursor_pos}, 
            type            {_type}, 
            mouse_scroll    { _mouse_scroll}, 
            modifiers       {modifier_state} 
    {
    };

    InputEvent( GLFWwindow* _glfw_window,
                CursorPosition      cursor_pos,
                EventType _type, 
                KeyStrokeEvent _key_stroke, 
                KeyModifiers modifier_state)
        :   glfw_window     {_glfw_window}, 
            cursor_pos      {cursor_pos}, 
            type            {_type}, 
            keystroke       { _key_stroke}, 
            modifiers       {modifier_state} 
    {
    };


    bool is_type(EventType _type) { return _type == type ? true : false;}

    /** Is any of the mouse events (move, scroll, click). */
    bool is_mouse()
    {
        bool is_of_mouse_type = 
            (type == EventType::MouseButton) ||
            (type == EventType::MouseMove) ||
            (type == EventType::MouseScroll);

        return is_of_mouse_type ? true : false;
    }
    bool is_mouse_button() {return type == EventType::MouseButton ? true : false;};
    bool is_mouse_movement() {return type == EventType::MouseMove ? true : false;};
    bool is_mouse_scroll() {return type == EventType::MouseScroll ? true : false;};
    bool is_keystroke() {return type == EventType::Keystroke ? true : false;};
};



}






/** 
	InputEvent-response intended to be processed by event deistributor.
*/
struct InputResponse
{
	/** Respondents mouse action. */
    enum MouseAction {
        MOUSE_GRAB,
        MOUSE_RELEASE,
    }; 

	/** Respondents keyboard action. */
    enum KeyboardAction {
		KEYBOARD_GRAB,
        KEYBOARD_RELEASE,
    }; 

	

	/** The type of action respondent performed on mouse. */
	MouseAction mouse_action    	= MOUSE_RELEASE;
	/** The type of action respondent performed on keyboard. */
	KeyboardAction keyboard_action 	= KEYBOARD_RELEASE;


	/** Defaults to no subsystem and all devices released. */
    InputResponse() = default;
    
	InputResponse(MouseAction mouse_action)
		: 	mouse_action {mouse_action} 
	{
	};
	
	InputResponse(KeyboardAction keyboard_action)
		: 	keyboard_action {keyboard_action} 
	{
	};
	
	InputResponse(MouseAction mouse_action, KeyboardAction keyboard_action)
		: 	mouse_action {mouse_action}, 
			keyboard_action {keyboard_action} 
	{
	};


    inline bool grabbed_mouse()
	{
		return mouse_action == MOUSE_GRAB ? true : false;
	}

    inline bool released_mouse()
	{
		return mouse_action == MOUSE_RELEASE ? true : false;
	}

	inline bool grabbed_keyboard()
	{
		return keyboard_action == KEYBOARD_GRAB ? true : false;
	}

    inline bool released_keyboard()
	{
		return keyboard_action == KEYBOARD_RELEASE ? true : false;
	}
};



/** Keeps track of which subsystems is currently grabbing. */
struct InputState 
{
	InputResponse last_response;


	void update_state(InputResponse response)
	{
		last_response = response;	
	}

	bool is_grabbing_mouse()
	{
		return last_response.mouse_action == InputResponse::MOUSE_GRAB ? true : false;
	}

    bool is_grabbing_keyboard()
	{
		return last_response.keyboard_action == InputResponse::KEYBOARD_GRAB ? true : false;
	}

    bool is_all_release()
    {
        return !is_grabbing_mouse() && !is_grabbing_keyboard() ? true : false;
    }

};