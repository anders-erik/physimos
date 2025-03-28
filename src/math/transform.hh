
#pragma once

#include "vecmat.hh"



struct Transform {
    f3 pos;
    f3 rot;
    m4f4 matrix;
    m4f4 matrix_anim;
};

