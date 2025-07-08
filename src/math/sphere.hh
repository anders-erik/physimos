#pragma once


#include "math/vecmat.hh"




struct Sphere
{
    f3 pos;
    float r;

    Sphere() = default;

    void update(f3 new_pos) { pos = new_pos; }

};

