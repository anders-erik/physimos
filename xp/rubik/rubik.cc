
#include "math.hh"
#include "model.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "rubik.hh"

namespace xprubik {


Cube::Cube() {

    model_add_cube_mesh(c0.model.mesh);
    c0.model.transform.pos.x = 1.0f;
    model_add_cube_mesh(c1.model.mesh);
    c1.model.transform.pos.x = 1.0f;
    c1.model.transform.pos.y = 1.0f;

};

void Cube::handle_input(InputState input_state){

    if(animator.is_animating)
        return;


    if(input_state.f && input_state.shift){
        c0.c_rot.pos_x();
        c1.c_rot.pos_x();

        animator.is_animating = true;
        animator.permutation = Permutation::Fi;
        
    }
    else if(input_state.f){
        c0.c_rot.neg_x();
        c1.c_rot.neg_x();

        animator.is_animating = true;
        animator.permutation = Permutation::F;
    }

    if(input_state.r && input_state.shift){
        c0.model.transform.rot.y -= 0.1;
        c1.model.transform.rot.y -= 0.1;
    }
    else if(input_state.r){
        c0.model.transform.rot.y += 0.1;
        c1.model.transform.rot.y += 0.1;
    }


    if(input_state.p){
        print();
    }


    // ARROW LOOK AROUND
    if(input_state.up){
        c0.model.transform.rot.x += 0.05;
        c1.model.transform.rot.x += 0.05;
    }
    if(input_state.down){

        c0.model.transform.rot.x -= 0.05;
        c1.model.transform.rot.x -= 0.05;
    }
    if(input_state.left){
        c0.model.transform.rot.z += 0.05;
    }
    if(input_state.right){
        c0.model.transform.rot.z -= 0.05;
    }

}

void Cube::update_animator(){

    if(!animator.is_animating)
        return;

    ++animator.current_frame_count;

    if(animator.permutation == Permutation::F){
        c0.model.transform.rot.x -= animator.animation_angle_step;
        c1.model.transform.rot.x -= animator.animation_angle_step;
    }
    else if(animator.permutation == Permutation::Fi){
        c0.model.transform.rot.x += animator.animation_angle_step;
        c1.model.transform.rot.x += animator.animation_angle_step;
    }


    if(animator.current_frame_count > animator.frames_per_anim){
        animator.is_animating = false;
        animator.current_frame_count = 0;

        c0.model.transform.rot.x = c0.c_rot.get_x_rad();
        c1.model.transform.rot.x = c1.c_rot.get_x_rad();
    }
}

void CubieRotation::pos_x(){
    switch (x)
    {
    case Rot90::A0 :
        x = Rot90::A90;
        break;
    case Rot90::A90 :
        x = Rot90::A180;
        break;
    case Rot90::A180 :
        x = Rot90::A270;
        break;
    case Rot90::A270 :
        x = Rot90::A0;
        break;
    
    default:
        break;
    }
}
void CubieRotation::neg_x(){
    switch (x)
    {
    case Rot90::A0 :
        x = Rot90::A270;
        break;
    case Rot90::A90 :
        x = Rot90::A0;
        break;
    case Rot90::A180 :
        x = Rot90::A90;
        break;
    case Rot90::A270 :
        x = Rot90::A180;
        break;
    
    default:
        break;
    }
}

float CubieRotation::get_x_rad(){
    float return_value;

    switch (x)
    {
    case Rot90::A0 :
        return_value = 0.0f;
        break;
    case Rot90::A90 :
        return_value = 1.57f;
        break;
    case Rot90::A180 :
        return_value = 3.14f;
        break;
    case Rot90::A270 :
        return_value = 4.71f;
        break;
    
    default:
        break;
    }

    return return_value;
}



void Cube::print(){
    std::cout << "c0.model.transform.rot.x = " << c0.model.transform.rot.x << std::endl;
    std::cout << "c0.model.transform.rot.y = " << c0.model.transform.rot.y << std::endl;
    std::cout << "c0.model.transform.rot.z = " << c0.model.transform.rot.z << std::endl;

}

}
