
#pragma once

#include "vecmat.hh"


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

    void translate(f3 transl);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);
    
    static void translate(m4f4& matrix, f3 transl);
    static void rotate_x(m4f4& matrix, float angle);
    static void rotate_y(m4f4& matrix, float angle);
    static void rotate_z(m4f4& matrix, float angle);

    static m4f4 create_translation(f3 transl);
    static m4f4 create_rotation_x(float angle);
    static m4f4 create_rotation_y(float angle);
    static m4f4 create_rotation_z(float angle);



    /** Rotates a unit vector in the f3.x direction to point in the direction of the passed rectangular f3 vector.  */
    static m4f4 rect_f3_to_m4f4(f3 rect_vector);
};

