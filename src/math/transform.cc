
#include "transform.hh"

#include <cmath>


void Transform::set_matrix_model()
{
    m4f4 X = m4f4::rotation_x(rot.x);
    m4f4 Y = m4f4::rotation_y(rot.y);
    m4f4 Z = m4f4::rotation_z(rot.z);

    m4f4 T = m4f4::translation(pos);

    matrix = X * Y * Z * T;
}









YawPitch Transform::
to_yaw_pitch(f3 rect_vector)
{
    f2 vector_xy = rect_vector.to_xy();
    
    // Pure z-vectors need xy component to be visible
    if(vector_xy.is_zero())
        rect_vector.y = 0.0001f;

    float yaw = vector_xy.angle();
    float pitch = atanf( rect_vector.z / vector_xy.norm());

    return {yaw, pitch};
}

m4f4 Transform::yaw_pitch_matrix(YawPitch YP)
{
    m4f4 YAW = m4f4::rotation_z(-YP.yaw());
    m4f4 PITCH = m4f4::rotation_y(-YP.pitch());
    return YAW * PITCH;
}


