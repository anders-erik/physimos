
#include "transform.hh"

#include <cmath>


void Transform2D::set_pos(float posx, float posy){
    pos.x = posx;
    pos.y = posy;
}
void Transform2D::set_scale(float scalex, float scaley){
    scale.x = scalex;
    scale.y = scaley;
}
void Transform2D::move_x(float dx){
    pos.x += dx;
}
void Transform2D::move_y(float dy){
    pos.y += dy;
}

void Transform2D::set_matrix_model(){

    // SCALE
    m3f3 scale_mat;
    scale_mat.scale(scale);

    // TRANSLATION
    m3f3 pos_mat;
    pos_mat.translate(pos);

    // MULTIPLY
    matrix.set_to_identity();
    matrix.mult_right(pos_mat);
    matrix.mult_right(scale_mat);


}
void Transform2D::set_matrix_camera(){

    // m3f3 identity;

    // SCALE
    m3f3 scale_mat;
    scale_mat.scale(scale);

    // TRANSLATION
    m3f3 pos_mat;
    f2 pos_neg = {-pos.x, -pos.y};
    pos_mat.translate(pos_neg);

    // MULTIPLY
    matrix.set_to_identity();
    matrix.mult_right(scale_mat);
    matrix.mult_right(pos_mat);
}



m4f4 Transform2D::get_m4f4(){
    m4f4 new_m4f4;

    new_m4f4.x.x = matrix.x.x;
    new_m4f4.x.y = matrix.x.y;
    new_m4f4.x.z = matrix.x.z;

    new_m4f4.y.x = matrix.y.x;
    new_m4f4.y.y = matrix.y.y;
    new_m4f4.y.z = matrix.y.z;

    new_m4f4.z.x = matrix.x.x;
    new_m4f4.z.y = matrix.x.y;
    new_m4f4.z.z = matrix.x.z;

    return new_m4f4;
}

void Transform::set_matrix_model()
{
    m4f4 X = m4f4::rotation_x(rot.x);
    m4f4 Y = m4f4::rotation_y(rot.y);
    m4f4 Z = m4f4::rotation_z(rot.z);

    m4f4 T = m4f4::translation(pos);

    matrix = X * Y * Z * T;
}









f2 Transform::
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

m4f4 Transform::yaw_pitch_matrix(float yaw, float pitch)
{
    m4f4 YAW = m4f4::rotation_z(-yaw);
    m4f4 PITCH = m4f4::rotation_y(-pitch);
    return YAW * PITCH;
}


m4f4 Transform::
rect_f3_to_m4f4(f3 rect_vector)
{
    f2 vector_xy = rect_vector.to_xy();
    
    // Pure z-vectors need xy component to be visible
    if(vector_xy.is_zero())
        rect_vector.y = 0.0001f;

    float yaw = vector_xy.angle();
    m4f4 YAW = m4f4::rotation_z(-yaw);

    float pitch = atanf( rect_vector.z / vector_xy.norm());
    m4f4 PITCH = m4f4::rotation_y(-pitch);

    return YAW * PITCH;
}

