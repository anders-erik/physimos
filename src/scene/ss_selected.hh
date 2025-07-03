#pragma once

#include "math/vecmat.hh"

#include "window/auxevent.hh"

#include "scene/tago.hh"


namespace SS 
{

struct Selected
{
    typedef unsigned char State;
    
    static const State EMPTY        = 0x00;
    static const State SELECTED     = 0x01;
    static const State KEY_1        = 0x02;
    static const State KEY_2        = 0x04;


    TagO    tag;
    State   state       = EMPTY;
    Key     key_1       = Key::None;
    Key     key_2       = Key::None;

    f3      pos_delta;
    f3      rot_delta_norm;
    float   size_factor;


    bool is_null() { return tag.is_null(); }
    bool has_key()
    {
        return state > SELECTED;
    }

    void try_select(TagO new_tag_click) 
    {
        clear();
        if(new_tag_click.is_null())
            return;
        tag = new_tag_click; 
        state = SELECTED;
    }

    void clear()
    {
        tag = TagO();
        state = EMPTY;
        key_1 = Key::None;
        key_2 = Key::None;
    }

    bool try_peel() 
    {
        switch (state)
        {
        case KEY_2:
            state = KEY_1;
            key_2 = Key::None;
            break;
        case KEY_1:
            state = SELECTED;
            key_1 = Key::None;
            break;
        case SELECTED:
            clear();
            break;
        default:
            return false;
            break;
        }
        return true;
    }

    void set_key_1(Key key)
    {
        state = KEY_1;
        key_1 = key;
    }

    bool handle_selected(window::InputEvent& event)
    {
        if(state != SELECTED)
            throw std::runtime_error("Tried to handle selected state when not selected");


        return false;
    }
};


}

