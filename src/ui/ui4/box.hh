
#pragma once

#include "math/vecmat.hh"


struct Box
{
    d2 pos; // position
    d2 size; // size

    Box() {}
    Box(d2 _pos, d2 _size) : pos {_pos}, size {_size} {}

    bool contains(d2 _p)
    {
        bool x_check = _p.x > pos.x && _p.x < pos.x + size.x;
        bool y_check = _p.y > pos.y && _p.y < pos.y + size.y;

        return x_check && y_check;
    }
};  