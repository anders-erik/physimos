
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


    f3 red =   f3(0.5f, 0.0f, 0.0f);
    f3 green = f3(0.0f, 0.5f, 0.0f);
    f3 blue =  f3(0.0f, 0.0f, 0.5f);
    f3 orange = f3(0.5f, 0.25f, 0.0f);
    f3 yellow = f3(0.5f, 0.5f, 0.0f);
    f3 white =  f3(0.8f, 0.8f, 0.8f);

    f3 f_color = green;
    f3 b_color = blue;
    f3 r_color = red;
    f3 l_color = orange;
    f3 u_color = yellow;
    f3 d_color = white;


    Cubie& f = cubies[0];
    Cubie& fr = cubies[1];
    Cubie& fl = cubies[2];
    Cubie& fu = cubies[3];
    Cubie& fd = cubies[4];

    Cubie& rb = cubies[5];
    Cubie& ru = cubies[6];
    Cubie& rd = cubies[7];
    

    // F CENTER
    model_add_cube_mesh(f.model.mesh);
    model_add_facelet(f.model.mesh, f_color, Axis::x);
    f.type = CubieType::Center;
    f.faces.one = Face::F;

    model_add_cube_mesh(fr.model.mesh);
    model_add_facelet(fr.model.mesh, f_color, Axis::x);
    model_add_facelet(fr.model.mesh, r_color, Axis::y);
    fr.type = CubieType::Edge;
    fr.faces.one = Face::F;
    fr.faces.two = Face::R;

    model_add_cube_mesh(fl.model.mesh);
    model_add_facelet(fl.model.mesh, f_color, Axis::x);
    model_add_facelet(fl.model.mesh, l_color, Axis::ny);
    fl.type = CubieType::Edge;
    fl.faces.one = Face::F;
    fl.faces.two = Face::L;

    model_add_cube_mesh(fu.model.mesh);
    model_add_facelet(fu.model.mesh, f_color, Axis::x);
    model_add_facelet(fu.model.mesh, u_color, Axis::z);
    fu.type = CubieType::Edge;
    fu.faces.one = Face::F;
    fu.faces.two = Face::U;

    model_add_cube_mesh(fd.model.mesh);
    model_add_facelet(fd.model.mesh, f_color, Axis::x);
    model_add_facelet(fd.model.mesh, d_color, Axis::nz);
    fd.type = CubieType::Edge;
    fd.faces.one = Face::F;
    fd.faces.two = Face::D;




    model_add_cube_mesh(rb.model.mesh);
    model_add_facelet(rb.model.mesh, r_color, Axis::y);
    model_add_facelet(rb.model.mesh, b_color, Axis::nx);
    rb.type = CubieType::Edge;
    rb.faces.one = Face::R;
    rb.faces.two = Face::B;

    model_add_cube_mesh(ru.model.mesh);
    model_add_facelet(ru.model.mesh, r_color, Axis::y);
    model_add_facelet(ru.model.mesh, u_color, Axis::z);
    ru.type = CubieType::Edge;
    ru.faces.one = Face::R;
    ru.faces.two = Face::U;

    model_add_cube_mesh(rd.model.mesh);
    model_add_facelet(rd.model.mesh, r_color, Axis::y);
    model_add_facelet(rd.model.mesh, d_color, Axis::nz);
    rd.type = CubieType::Edge;
    rd.faces.one = Face::R;
    rd.faces.two = Face::D;


    model_add_cube_mesh(c_xp.model.mesh);
    // c_xp.model.set_base_color(green);
    // c_xp.model.transform.pos.x = 0.0f;
    // c_xp.model.transform.pos.y = 1.0f;
    // c_xp.model.transform.pos.z = 1.0f;
    model_add_facelet(c_xp.model.mesh, green, Axis::x);
    model_add_facelet(c_xp.model.mesh, blue, Axis::nx);
    model_add_facelet(c_xp.model.mesh, red, Axis::y);
    model_add_facelet(c_xp.model.mesh, orange, Axis::ny);
    model_add_facelet(c_xp.model.mesh, yellow, Axis::z);
    model_add_facelet(c_xp.model.mesh, white, Axis::nz);
    c_xp.type = CubieType::Edge;
    c_xp.faces.one = Face::R;
    c_xp.faces.two = Face::U;
    // Initial matrix:
    m4f4 identity;
    m4f4 initial_pos_mat = m4f4_create_translation(c_xp.model.transform.pos);
    mat_mul(c_xp.model.transform.matrix, initial_pos_mat);
    // c_xp.model.transform.matrix = identity;


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

void Cubie::set_position_from_faces(){

    if(faces.contains(Face::F)){
        model.transform.pos.x = 1.0f;
    }
    if(faces.contains(Face::B)){
        model.transform.pos.x = -1.0f;
    }
    if(faces.contains(Face::R)){
        model.transform.pos.y = 1.0f;
    }
    if(faces.contains(Face::L)){
        model.transform.pos.y = -1.0f;
    }
    if(faces.contains(Face::U)){
        model.transform.pos.z = 1.0f;
    }
    if(faces.contains(Face::D)){
        model.transform.pos.z = -1.0f;
    }

}
void Cubie::set_rotation_transform_from_discrete_rot(){

    model.transform.rot.x = c_rot.get_rad(c_rot.x);
    model.transform.rot.y = c_rot.get_rad(c_rot.y);
    model.transform.rot.z = c_rot.get_rad(c_rot.z);

}
void Cube::update_cubies(){



    for(Cubie& c : cubies){
        c.model.transform.pos = f3();
        c.model.transform.rot = f3();
        c.set_position_from_faces();
        c.set_rotation_transform_from_discrete_rot();
        c.model.set_transform_matrix();
    }
}

int count = 0;
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

    if(input_state.left){
        // c_xp.model.transform.pos.x += 0.01f;
        // c_xp.model.transform.rot.x += 0.1f;
        float angle = 1.5707825f;
        m4f4 identity;
        m4f4 x_rot_mat = m4f4_create_rotation_x(angle);
        m4f4 current_tmp = c_xp.model.transform.matrix;
        c_xp.model.transform.matrix = identity;
        mat_mul(c_xp.model.transform.matrix, x_rot_mat);
        mat_mul(c_xp.model.transform.matrix, current_tmp);
        
        // std::cout << "count = " << ++count << std::endl;
        
    }
    else if(input_state.down){
        float angle = 1.5707825f;
        m4f4 identity;
        m4f4 y_rot_mat = m4f4_create_rotation_y(angle);
        m4f4 current_tmp = c_xp.model.transform.matrix;
        c_xp.model.transform.matrix = identity;
        mat_mul(c_xp.model.transform.matrix, y_rot_mat);
        mat_mul(c_xp.model.transform.matrix, current_tmp);
    }
    else if(input_state.right){}

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
