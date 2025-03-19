
#include "math.hh"
#include "model.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "rubik.hh"

namespace xprubik {


void Faces::permute_f_edge(Permutation p){
    
    Face& non_f = (one == Face::F) ? two : one;

    if(non_f == Face::R)
        non_f = Face::D;
    else if(non_f == Face::U)
        non_f = Face::R;
    else if(non_f == Face::L)
        non_f = Face::U;
    else if(non_f == Face::D)
        non_f = Face::L;
    
}
void Faces::permute_fi_edge(Permutation p){

    Face& non_f = (one == Face::F) ? two : one;

    if(non_f == Face::R)
        non_f = Face::U;
    else if(non_f == Face::U)
        non_f = Face::L;
    else if(non_f == Face::L)
        non_f = Face::D;
    else if(non_f == Face::D)
        non_f = Face::R;

}
void Faces::permute_r_edge(Permutation p){
    
    Face& non_r = (one == Face::R) ? two : one;

    if(non_r == Face::B)
        non_r = Face::D;
    else if(non_r == Face::U)
        non_r = Face::B;
    else if(non_r == Face::F)
        non_r = Face::U;
    else if(non_r == Face::D)
        non_r = Face::F;


}
void Faces::permute_ri_edge(Permutation p){

    Face& non_r = (one == Face::R) ? two : one;

    if(non_r == Face::B)
        non_r = Face::U;
    else if(non_r == Face::U)
        non_r = Face::F;
    else if(non_r == Face::F)
        non_r = Face::D;
    else if(non_r == Face::D)
        non_r = Face::B;
}


bool Faces::contains(Face face){

    if      (one == face)
        return true;
    else if (two == face)
        return true;
    else if (three == face)
        return true;

    return false;
}


Cube::Cube() {

    model_add_cube_mesh(cubies[0].model.mesh);
    cubies[0].model.transform.pos.x = 1.0f;
    cubies[0].type = CubieType::Center;
    cubies[0].faces.one = Face::F;

    model_add_cube_mesh(cubies[1].model.mesh);
    cubies[1].model.transform.pos.x = 1.0f;
    cubies[1].model.transform.pos.y = 1.0f;
    cubies[1].type = CubieType::Edge;
    cubies[1].faces.one = Face::F;
    cubies[1].faces.two = Face::R;

};

void Cube::permute(Permutation permutation){

    // Start Animator
    animator.is_animating = true;
    animator.permutation = permutation;


    for(Cubie& c : cubies){


        switch (permutation)
        {
            case Permutation::F:
                // Only permute if face is part of current twist
                if(!c.faces.contains(Face::F))
                    continue;
                
                c.is_animating = true;

                if(c.type == CubieType::Edge)
                    c.faces.permute_f_edge(permutation);

                c.c_rot.x = c.c_rot.neg(c.c_rot.x);
                
                break;

            case Permutation::Fi:
                // Only permute if face is part of current twist
                if(!c.faces.contains(Face::F))
                    continue;

                c.is_animating = true;

                if(c.type == CubieType::Edge)
                    c.faces.permute_fi_edge(permutation);

                c.c_rot.x = c.c_rot.pos(c.c_rot.x);
                break;

            case Permutation::R:
                // Only permute if face is part of current twist
                if(!c.faces.contains(Face::R))
                    continue;

                c.is_animating = true;

                if(c.type == CubieType::Edge)
                    c.faces.permute_r_edge(permutation);

                c.c_rot.y = c.c_rot.neg(c.c_rot.y);
                break;

            case Permutation::Ri:
                // Only permute if face is part of current twist
                if(!c.faces.contains(Face::R))
                    continue;

                c.is_animating = true;

                if(c.type == CubieType::Edge)
                    c.faces.permute_ri_edge(permutation);

                c.c_rot.y = c.c_rot.pos(c.c_rot.y);
                break;
        
        default:
            break;
        }

    }

}

void Cube::handle_input(InputState input_state){

    if(animator.is_animating)
        return;


    if(input_state.f && input_state.shift)
        permute(Permutation::Fi);
    else if(input_state.f)
        permute(Permutation::F);
    else if(input_state.r && input_state.shift)
        permute(Permutation::Ri);
    else if(input_state.r)
        permute(Permutation::R);



    if(input_state.p){
        print();
    }

}




void Cubie::update_animation(Permutation perm, float angle_delta){

    if(!is_animating)
        return;

    if(perm == Permutation::F){
        model.transform.rot.x -= angle_delta;

    }
    else if(perm == Permutation::Fi){
        model.transform.rot.x += angle_delta;
    }
    else if(perm == Permutation::R){
        model.transform.rot.y -= angle_delta;

    }
    else if(perm == Permutation::Ri){
        model.transform.rot.y += angle_delta;

    }

}


void Cube::update_animator(){

    if(!animator.is_animating)
        return;

    ++animator.current_frame_count;


    for(Cubie& c : cubies){
        c.update_animation(animator.permutation, animator.animation_angle_step);
    }

    // END CONDITION
    if(animator.current_frame_count > animator.frames_per_anim){

        animator.is_animating = false;
        animator.current_frame_count = 0;

        for(Cubie& c : cubies){
            c.is_animating = false;
            // Set end rotation to fixed value. Prevents compounding rotation errors.
            c.model.transform.rot.x = c.c_rot.get_rad(c.c_rot.x);
            c.model.transform.rot.y = c.c_rot.get_rad(c.c_rot.y);
            c.model.transform.rot.z = c.c_rot.get_rad(c.c_rot.z);
        }
        
        
    }
}

Rot90 CubieRotation::pos(Rot90 rot){
    Rot90 return_rot;

    switch (rot){

        case Rot90::A0 :
            return_rot = Rot90::A90;
            break;
        case Rot90::A90 :
            return_rot = Rot90::A180;
            break;
        case Rot90::A180 :
            return_rot = Rot90::A270;
            break;
        case Rot90::A270 :
            return_rot = Rot90::A0;
            break;

    }

    return return_rot;
}
Rot90 CubieRotation::neg(Rot90 rot){
    Rot90 return_rot;

    switch (rot){

        case Rot90::A0 :
            return_rot = Rot90::A270;
            break;
        case Rot90::A90 :
            return_rot = Rot90::A0;
            break;
        case Rot90::A180 :
            return_rot = Rot90::A90;
            break;
        case Rot90::A270 :
            return_rot = Rot90::A180;
            break;
            
    }

    return return_rot;
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

float CubieRotation::get_rad(Rot90 rot){
    float return_value;

    switch (rot)
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
    std::cout << "cubies[0].model.transform.rot.x = " << cubies[0].model.transform.rot.x << std::endl;
    std::cout << "cubies[0].model.transform.rot.y = " << cubies[0].model.transform.rot.y << std::endl;
    std::cout << "cubies[0].model.transform.rot.z = " << cubies[0].model.transform.rot.z << std::endl;

    // std::cout << "cubies[0].model.transform.rot.x = " << cubies[0].model.transform.rot.x << std::endl;

    // std::cout << "cubies[1].model.transform.rot.y = " << cubies[1].faces.one << std::endl;
    // std::cout << "cubies[1].model.transform.rot.z = " << cubies[1].faces.two << std::endl;
}

}
