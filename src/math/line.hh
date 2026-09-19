
#pragma once

#include "math/vecmat.hh"


// y = kx + m
class Line
{
public:

    d2 p1;
    d2 p2;

    double k;
    double m;

    Line(d2 _p1, d2 _p2) : p1 {_p1}, p2 {_p2}
    {
        // y = kx + m
        k = (p2.y - p1.y)/(p2.x - p1.x);
        m = p1.y - p1.x*k;
    }

    double operator[](double _x)
    {
        return k*_x + m;
    }

};
