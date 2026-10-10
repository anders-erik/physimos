#pragma once

#include "lib/defs.hh"
#include "lib/str.hh"


/** each component is f64 -- 128 bit total size */
struct c64
{
    f64 r = 0.0;
    f64 i = 0.0;

    c64()
    {
    }

    c64(f64 _r, f64 _i)
    {
        r = _r;
        i = _i;
    }

    c64(Str _str)
    {
        // Parse string
    }

    c64 operator+(c64 rhs)
    {
        return c64{r+rhs.r, i+rhs.i};
    }
    c64 operator-(c64 rhs)
    {
        return c64{this->r+rhs.r, this->i+rhs.i};
    }
    
    Str to_str()
    {
        Str str;
        str += Str::FL(r, 3, Str::FloatRep::Fixed);
        str += " + ";
        str += Str::FL(i, 3, Str::FloatRep::Fixed);
        str += "i";
        return str;
    }
};

