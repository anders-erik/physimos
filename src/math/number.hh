
#pragma once

#include "str.hh"

/** Real 2D point.
    R^2 = (R, R) */
template <typename F>
struct R2
{
    F x;
    F y;

    bool operator==(R2 rhs) {return (x == rhs.x && y == rhs.y); }
    F* data() { return (F*) this; }

    Str to_str()
    {
        return Str{"[", Str::Num(x), ", ", Str::Num(x), "]"};
    }
};


