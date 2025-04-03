
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
    matrix.mult(pos_mat);
    matrix.mult(scale_mat);


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
    matrix.mult(scale_mat);
    matrix.mult(pos_mat);
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
    matrix.rotate_x(rot.x);
    matrix.rotate_y(rot.y);
    matrix.rotate_z(rot.z);

    matrix.translate(pos);



}

namespace math {



m4f4 rect_f3_to_m4f4(f3 rect_vector){

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
    
    rotation_mat.rotate_z(-yaw);    // NOTE: Not sure why the signs should both be negative, 
    rotation_mat.rotate_y(-pitch);  // Yet the vectors appear to be displayed correctly when this is the case...

    return rotation_mat;
}



}
