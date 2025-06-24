#pragma once

#include "lib/str.hh"

#include "window/key.hh"
#include "window/key_operators.hh"


namespace window {


struct KeyModifiers
{
    Key state = Key::None;


    KeyModifiers() = default;

    void press(Key new_mod_key)     { state |= new_mod_key;}
    void release(Key new_mod_key)   { state ^= new_mod_key;}

    void set_shift()        { state |= Key::shift; }
    void unset_shift()      { state ^= Key::shift; }

    void set_ctrl()         { state |= Key::ctrl; }
    void unset_ctrl()       { state ^= Key::ctrl; }

    void set_alt()          { state |= Key::alt; }
    void unset_alt()        { state ^= Key::alt; }


    bool is_none()          const   { return state ==  Key::None ; }
    bool has_state()        const   { return state !=  Key::None ; }

    bool is_shift()         const   { return (state & Key::shift) == Key::shift     ; }
    bool is_ctrl()          const   { return (state & Key::ctrl ) == Key::ctrl      ; }
    bool is_alt()           const   { return (state & Key::alt  ) == Key::alt       ; }
    bool is_esc()           const   { return (state & Key::Esc  ) == Key::Esc       ; }

    bool is_ctrl_shift()    const   { return is_ctrl()  && is_shift()                   ; }
    bool is_alt_shift()     const   { return is_alt()   && is_shift()                   ; }
    bool is_alt_ctrl()      const   { return is_alt()   && is_ctrl()                    ; }
    bool is_alt_ctrl_shift()const   { return is_alt()   && is_ctrl()    && is_shift()   ; }

    /** Note that `esc` is an axuillary mod key; useful for the occational misc state handling. */
    static bool is_mod_key(Key key_to_query) 
    {
        return key_to_query == Key::shift   ||
               key_to_query == Key::ctrl    ||
               key_to_query == Key::alt     ||
               key_to_query == Key::Esc;
    }



    static Str to_str(KeyModifiers& modifiers)
    {
        Str str;
        switch (modifiers.state)
        {

        case Key::None:
            str = "None";
            break;
        case Key::shift:
            str = "Shift";
            break;
        case Key::ctrl:
            str = "Ctrl";
            break;
        case Key::ctrl_shift:
            str = "Ctrl + Shift";
            break;
        case Key::alt:
            str = "Alt";
            break;
        case Key::alt_shift:
            str = "Alt + Shift";
            break;
        case Key::alt_ctrl:
            str = "Alt + Ctrl";
            break;
        case Key::alt_ctrl_shift:
            str = "Alt + Ctrl + Shift";
            break;
        case Key::Esc:
            str = "Esc";
            break;
        
        default:
            break;
        }

        return str;
    }
};



}
