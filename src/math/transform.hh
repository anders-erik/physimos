
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


struct Transform2D {
    f2 pos;
    f2 scale;
    f2 rot;
    m3f3 matrix;

    float z;

    void set_pos(float posx, float posy);   
    void set_scale(float scalex, float scaley);
    void move_x(float dx);
    void move_y(float dy);

    void set_matrix_model();
    void set_matrix_camera();
    m4f4 get_m4f4();
};


struct Transform {
    f3 pos;
    f3 rot;
    m4f4 matrix;

    void set_matrix_model(); // Old transform interface

    static YawPitch to_yaw_pitch(f3 rect_vector);
    static m4f4 yaw_pitch_matrix(YawPitch YP);
};

