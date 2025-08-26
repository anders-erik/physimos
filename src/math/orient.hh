#pragma once

#include "math/vecmat.hh"

/**
    If returns positive, C is to the left of line from A to B
*/
float Orient2D(f2 A, f2 B, f2 C)
{
    m2f2 mat { 
        {A.x - C.x, A.y - C.y}, 
        {B.x - C.x, B.y - C.y}
    };
    return mat.det();
}