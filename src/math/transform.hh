
#pragma once

#include "vecmat.hh"



struct Transform {
    f3 pos;
    f3 rot;
    m4f4 matrix;

    void set_matrix_model();
};

