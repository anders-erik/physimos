
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
};


