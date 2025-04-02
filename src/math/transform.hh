
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

    void set_matrix_model();
};

namespace math {

struct SphericalCoord {
    // f3 rect;
    // f3 sphere;
};

/** Returns: rotation matrix that will rotate a unit vector in the f3.x direction to point in the direction of the passed rectangular f3 vector.  */
m4f4 rect_f3_to_m4f4(f3 rect_vector);


}