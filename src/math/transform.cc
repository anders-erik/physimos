
#include "transform.hh"



void Transform::set_matrix_model(){

    m4f4 identity;

    matrix = identity;

    
    // Intrinsic rotations work??
    matrix.rotate_x(rot.x);
    matrix.rotate_y(rot.y);
    matrix.rotate_z(rot.z);

    matrix.translate(pos);



}
