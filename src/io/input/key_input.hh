
#pragma once

#include "lib/str.hh"
#include "math/vecmat.hh"

#include "keys.hh"


enum class KeyButtonAction
{
    Press,
    Release,
    Hold,
};


struct KeyInput
{
    Keys key;
    KeyButtonAction action;

    KeyInput(Keys _key, KeyButtonAction _action) : key {_key}, action {_action} {}

    Str to_str()
    {
        Str str;

        switch (key)
        {
        
        case Keys::A: str = "a"; break;
        case Keys::B: str = "b"; break;
        case Keys::C: str = "c"; break;
        case Keys::D: str = "d"; break;
        case Keys::E: str = "e"; break;
        case Keys::F: str = "f"; break;
        case Keys::G: str = "g"; break;
        case Keys::H: str = "h"; break;
        case Keys::I: str = "i"; break;
        case Keys::J: str = "j"; break;
        case Keys::K: str = "k"; break;
        case Keys::L: str = "l"; break;
        case Keys::M: str = "m"; break;
        case Keys::N: str = "n"; break;
        case Keys::O: str = "o"; break;
        case Keys::P: str = "p"; break;
        case Keys::Q: str = "q"; break;
        case Keys::R: str = "r"; break;
        case Keys::S: str = "s"; break;
        case Keys::T: str = "t"; break;
        case Keys::U: str = "u"; break;
        case Keys::V: str = "v"; break;
        case Keys::W: str = "w"; break;
        case Keys::X: str = "x"; break;
        case Keys::Y: str = "y"; break;
        case Keys::Z: str = "z"; break;
            
        default:
            break;
        }
        return str;
    }
};


