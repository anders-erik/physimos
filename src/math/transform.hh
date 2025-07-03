
#pragma once

#include "vecmat.hh"

struct YawPitch : public f2
{
    float yaw() {return x;}
    float pitch() {return y;}

    YawPitch(float yaw, float pitch)
        : f2(yaw, pitch)
    {};
};


struct Transform {
    f3 pos;
    f3 rot;
    m4f4 matrix;

    void set_matrix_model(); // Old transform interface

    static YawPitch to_yaw_pitch(f3 rect_vector);
    static m4f4 yaw_pitch_matrix(YawPitch YP);
};

