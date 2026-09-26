
#pragma once


#include "math/vecmat.hh"



enum class KeyButtonAction
{
    Press,
    Release,
    Hold,
};


enum class Keys
{
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,

    Backspace,
};



struct KeyPress
{
    Keys key;
    KeyButtonAction action;

    KeyPress(Keys _key, KeyButtonAction _action) : key {_key}, action {_action} {}
};


