
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

void Transform::set_matrix_model(){

    m4f4 identity;

    matrix = identity;

    
    // Intrinsic rotations work??
    rotate_y(rot.y);
    rotate_z(rot.z);
    rotate_x(rot.x);

    translate(pos);
}



void Transform::
translate(f3 transl)
{
    m4f4 tranls_matrix = create_translation(transl);

    // mat_mul(matrix, tranls_matrix);
    matrix.mult_in_place(tranls_matrix);

    // m4f4 tmp_mat;
    // tmp_mat = tranls_matrix;
    // mat_mul(tmp_mat, tranls_matrix);
    // mat_mul(tmp_mat, *this);

    // *this = tmp_mat;

}


void Transform::
rotate_x(float angle)
{
    m4f4 rot_x_matrix = create_rotation_x(angle);
    matrix = matrix * rot_x_matrix;
}


void Transform::
rotate_y(float angle)
{
    m4f4 rot_y_matrix = create_rotation_y(angle);
    matrix.mult_in_place(rot_y_matrix);
}


void Transform::
rotate_z(float angle)
{
    m4f4 rot_z_matrix = create_rotation_z(angle);
    matrix.mult_in_place(rot_z_matrix);
}




void Transform::
translate(m4f4& matrix, f3 transl)
{
    matrix = matrix * Transform::create_translation(transl);
}

void Transform::
rotate_x(m4f4 & matrix, float angle_rad)
{
    matrix = matrix * Transform::create_rotation_x(angle_rad);
}

void Transform::
rotate_y(m4f4 & matrix, float angle)
{
    matrix = matrix * Transform::create_rotation_y(angle);
}

void Transform::
rotate_z(m4f4 & matrix, float angle)
{
    matrix = matrix * Transform::create_rotation_z(angle);
}



m4f4 Transform::
create_translation(f3 transl)
{
    return {{1, 0, 0, transl.x},
            {0, 1, 0, transl.y},
            {0, 0, 1, transl.z},
            {0, 0, 0, 1       }};
}


m4f4 Transform::
create_rotation_x(float angle)
{
    return {{1, 0,           0,             0},
            {0, cosf(angle), -sinf(angle),  0},
            {0, sinf(angle), cosf(angle),   0},
            {0, 0,           0,             1}};
}


m4f4 Transform::
create_rotation_y(float angle)
{
    return {{cosf(angle),   0,  sinf(angle),    0},
            {0,             1,  0,              0},
            {-sinf(angle),  0,  cosf(angle),    0},
            {0,             0,  0,              1}};
}


m4f4 Transform::
create_rotation_z(float angle)
{
    return {{ cosf(angle),  sinf(angle),    0,  0},
            {-sinf(angle),  cosf(angle),    0,  0},
            {0,             0,              1,  0},
            {0,             0,              0,  1}};
}








m4f4 Transform::
rect_f3_to_m4f4(f3 rect_vector)
{
    // ROTATION MATRIX
    // Calculate pitch + yaw using passed vector, then use extrinsic rotation to construct rotation vector

    // NOTE: ANTIPATTERN
    if(rect_vector.x == 0.0f && rect_vector.y == 0.0f)
        rect_vector.y = 0.0001f;

    f3 vector_xy = rect_vector;
    // vector_xy.z = 0.0f;

    float vector_xy_mag = sqrtf( vector_xy.x*vector_xy.x + vector_xy.y*vector_xy.y);


    float yaw = atanf(vector_xy.y / vector_xy.x);
    float pitch = atanf( rect_vector.z / vector_xy_mag);

    //  atan only outputs -pi/2 <= atan <= pi/2, thus rotate to other two quadrant if vector facing that direction
    // if(vector_xy.x < 0.0f)
    //     yaw += 3.141592;



    m4f4 rotation_mat;// = glm::mat4(1.0f);

    // rotation_mat = glm::rotate(rotation_mat, yaw, f3(0.0f, 0.0f, 1.0f));
    // rotation_mat = glm::rotate(rotation_mat, -pitch, f3(0.0f, 1.0f, 0.0f));
    
    Transform::rotate_z(rotation_mat, -yaw);    // NOTE: Not sure why the signs should both be negative, 
    Transform::rotate_y(rotation_mat, -pitch);  // Yet the vectors appear to be displayed correctly when this is the case...

    return rotation_mat;
}

