
#include "math.hh"
#include "model.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "rubik.hh"

namespace xprubik {




Cube::Cube() {


    f3 red =   f3(0.5f, 0.0f, 0.0f);
    f3 green = f3(0.0f, 0.5f, 0.0f);
    f3 blue =  f3(0.0f, 0.0f, 0.5f);
    f3 orange = f3(0.5f, 0.25f, 0.0f);
    f3 yellow = f3(0.5f, 0.5f, 0.0f);
    f3 white =  f3(0.9f, 0.9f, 0.9f);

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
    Cubie& fru = cubies[5];
    Cubie& frd = cubies[6];
    Cubie& flu = cubies[7];
    Cubie& fld = cubies[8];

    Cubie& r = cubies[9];
    Cubie& rb = cubies[10];
    Cubie& ru = cubies[11];
    Cubie& rd = cubies[12];
    Cubie& rbu = cubies[13];
    Cubie& rbd = cubies[14];
    

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

    model_add_cube_mesh(fru.model.mesh);
    model_add_facelet(fru.model.mesh, f_color, Axis::x);
    model_add_facelet(fru.model.mesh, r_color, Axis::y);
    model_add_facelet(fru.model.mesh, u_color, Axis::z);
    fru.type = CubieType::Corner;
    fru.faces.one = Face::F;
    fru.faces.two = Face::R;
    fru.faces.three = Face::U;

    model_add_cube_mesh(frd.model.mesh);
    model_add_facelet(frd.model.mesh, f_color, Axis::x);
    model_add_facelet(frd.model.mesh, r_color, Axis::y);
    model_add_facelet(frd.model.mesh, d_color, Axis::nz);
    frd.type = CubieType::Corner;
    frd.faces.one = Face::F;
    frd.faces.two = Face::R;
    frd.faces.three = Face::D;

    model_add_cube_mesh(flu.model.mesh);
    model_add_facelet(flu.model.mesh, f_color, Axis::x);
    model_add_facelet(flu.model.mesh, l_color, Axis::ny);
    model_add_facelet(flu.model.mesh, u_color, Axis::z);
    flu.type = CubieType::Corner;
    flu.faces.one = Face::F;
    flu.faces.two = Face::L;
    flu.faces.three = Face::U;

    model_add_cube_mesh(fld.model.mesh);
    model_add_facelet(fld.model.mesh, f_color, Axis::x);
    model_add_facelet(fld.model.mesh, l_color, Axis::ny);
    model_add_facelet(fld.model.mesh, d_color, Axis::nz);
    fld.type = CubieType::Corner;
    fld.faces.one = Face::F;
    fld.faces.two = Face::L;
    fld.faces.three = Face::D;



    // R
    model_add_cube_mesh(r.model.mesh);
    model_add_facelet(r.model.mesh, r_color, Axis::y);
    r.type = CubieType::Center;
    r.faces.one = Face::R;

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

    model_add_cube_mesh(rbu.model.mesh);
    model_add_facelet(rbu.model.mesh, r_color, Axis::y);
    model_add_facelet(rbu.model.mesh, b_color, Axis::nx);
    model_add_facelet(rbu.model.mesh, u_color, Axis::z);
    rbu.type = CubieType::Corner;
    rbu.faces.one = Face::R;
    rbu.faces.two = Face::B;
    rbu.faces.three = Face::U;

    model_add_cube_mesh(rbd.model.mesh);
    model_add_facelet(rbd.model.mesh, r_color, Axis::y);
    model_add_facelet(rbd.model.mesh, b_color, Axis::nx);
    model_add_facelet(rbd.model.mesh, d_color, Axis::nz);
    rbd.type = CubieType::Corner;
    rbd.faces.one = Face::R;
    rbd.faces.two = Face::B;
    rbd.faces.three = Face::D;






    model_add_cube_mesh(c_xp.model.mesh);
    // c_xp.model.set_base_color(green);
    c_xp.model.transform.pos.x = -2.0f;
    c_xp.model.transform.pos.y = -2.0f;
    c_xp.model.transform.pos.z = 1.0f;
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

// void Cubie::permute(Permutation permutation){

// }

void Cube::permute(Permutation permutation){

    // Start Animator
    animator.is_animating = true;
    animator.permutation = permutation;


    for(Cubie& c : cubies){


        switch (permutation)
        {
            case Permutation::F:
                if(!c.faces.contains(Face::F))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_f_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_f_corner(permutation);

                c.c_rot.rot(Axis::nx);
                break;

            case Permutation::Fi:
                if(!c.faces.contains(Face::F))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_fi_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_fi_corner(permutation);

                c.c_rot.rot(Axis::x);
                break;

            case Permutation::R:
                if(!c.faces.contains(Face::R))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_r_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_r_corner(permutation);

                c.c_rot.rot(Axis::ny);
                break;

            case Permutation::Ri:
                if(!c.faces.contains(Face::R))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_ri_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_ri_corner(permutation);

                c.c_rot.rot(Axis::y);

                break;
        
        default:
            break;
        }

        c.is_rotating = true;
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

void Cube::update_cubies(){

    // as long as the animator is not animating, we simply render the internal cube state
    // When animating, the cubes previous render state is preserved and manipulated.
    // The goal of animation is to smoothly transition visually to the new internal state.
    // Discontinutity between the final animation frame and next internal state. Try to get close match.
    if(!animator.is_animating){

        for(Cubie& c : cubies){

            // Reset
            c.model.transform.pos = f3();
            c.model.transform.rot = f3();

            // Update pos and rot vectors
            c.set_position_from_faces();
            // c.set_rotation_transform_from_discrete_rot();


            c.model.set_transform_matrix();

            mat_mul(c.model.transform.matrix, c.c_rot.matrix);
        }

    }
    else{


        update_animator();

        for(Cubie& c : cubies){

            // Extrinsic rotation of cubes we identified during permutation
            if (c.is_rotating) {

                // Extrinsic rotation for animation
                m4f4 rot_mat;

                switch(animator.permutation){

                    case Permutation::F :
                        rot_mat.rotate_x(-animator.animation_angle_step);
                        break;

                    case Permutation::Fi :
                        rot_mat.rotate_x(animator.animation_angle_step);
                        break;
                
                    case Permutation::R :
                        rot_mat.rotate_y(-animator.animation_angle_step);
                        break;

                    case Permutation::Ri :
                        rot_mat.rotate_y(animator.animation_angle_step);
                        break;
                
                }

                // pre multuiply = extrinsic rotation based on current transform matrix
                mat_mul(rot_mat, c.model.transform.matrix);
                c.model.transform.matrix = rot_mat;

            }
        }
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



void Cube::update_animator(){

    ++animator.current_frame_count;

    // END CONDITION
    if(animator.current_frame_count > animator.frames_per_anim){

        animator.is_animating = false;
        animator.current_frame_count = 0;

        for(Cubie& c : cubies){
            c.is_rotating = false;
        }
        
        
    }
}


void CubieRotation::rot(Axis axis){
    m4f4 permute_matrix;

    switch (axis)
    {
        case Axis::x :
            permute_matrix.y.y = 0.0f;
            permute_matrix.y.z = -1.0f;
            permute_matrix.z.y = 1.0f;
            permute_matrix.z.z = 0.0f;
            break;

        case Axis::nx :
            permute_matrix.y.y = 0.0f;
            permute_matrix.y.z = 1.0f;
            permute_matrix.z.y = -1.0f;
            permute_matrix.z.z = 0.0f;
            break;

        case Axis::y :
            permute_matrix.x.x = 0.0f;
            permute_matrix.x.z = 1.0f;
            permute_matrix.z.x = -1.0f;
            permute_matrix.z.z = 0.0f;
            break;

        case Axis::ny :
            permute_matrix.x.x = 0.0f;
            permute_matrix.x.z = -1.0f;
            permute_matrix.z.x = 1.0f;
            permute_matrix.z.z = 0.0f;
            break;

        case Axis::z :
            permute_matrix.x.x = 0.0f;
            permute_matrix.x.y = -1.0f;
            permute_matrix.y.x = 1.0f;
            permute_matrix.y.y = 0.0f;
            break;

        case Axis::nz :
            permute_matrix.x.x = 0.0f;
            permute_matrix.x.y = 1.0f;
            permute_matrix.y.x = -1.0f;
            permute_matrix.y.y = 0.0f;
            break;

    default:
        break;
    }

    mat_mul(permute_matrix, matrix);
    matrix = permute_matrix;
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

void Faces::permute_f(Face& face){
    if(face == Face::R)
        face = Face::D;
    else if(face == Face::U)
        face = Face::R;
    else if(face == Face::L)
        face = Face::U;
    else if(face == Face::D)
        face = Face::L;
}

void Faces::permute_fi(Face& face){
    if(face == Face::R)
        face = Face::U;
    else if(face == Face::U)
        face = Face::L;
    else if(face == Face::L)
        face = Face::D;
    else if(face == Face::D)
        face = Face::R;
}

void Faces::permute_r(Face& face){
    if(face == Face::B)
        face = Face::D;
    else if(face == Face::U)
        face = Face::B;
    else if(face == Face::F)
        face = Face::U;
    else if(face == Face::D)
        face = Face::F;
}

void Faces::permute_ri(Face& face){
    if(face == Face::B)
        face = Face::U;
    else if(face == Face::U)
        face = Face::F;
    else if(face == Face::F)
        face = Face::D;
    else if(face == Face::D)
        face = Face::B;
}

void Faces::permute_f_edge(Permutation p){
    
    Face& non_f = (one == Face::F) ? two : one;

    permute_f(non_f);

}

void Faces::permute_fi_edge(Permutation p){

    Face& non_f = (one == Face::F) ? two : one;

    permute_fi(non_f);

}

void Faces::permute_r_edge(Permutation p){
    
    Face& non_r = (one == Face::R) ? two : one;

    permute_r(non_r);


}
void Faces::permute_ri_edge(Permutation p){

    Face& non_r = (one == Face::R) ? two : one;

    permute_ri(non_r);
}


void Faces::permute_f_corner(Permutation p){

    if(one == Face::F){
        permute_f(two);
        permute_f(three);
    }
    else if(two == Face::F){
        permute_f(one);
        permute_f(three);
    }
    else if(three == Face::F) {
        permute_f(one);
        permute_f(two);
    }

}

void Faces::permute_fi_corner(Permutation p){
    if(one == Face::F){
        permute_fi(two);
        permute_fi(three);
    }
    else if(two == Face::F){
        permute_fi(one);
        permute_fi(three);
    }
    else if(three == Face::F) {
        permute_fi(one);
        permute_fi(two);
    }
}
void Faces::permute_r_corner(Permutation p){
    if(one == Face::R){
        permute_r(two);
        permute_r(three);
    }
    else if(two == Face::R){
        permute_r(one);
        permute_r(three);
    }
    else if(three == Face::R) {
        permute_r(one);
        permute_r(two);
    }
}
void Faces::permute_ri_corner(Permutation p){

    if(one == Face::R){
        permute_ri(two);
        permute_ri(three);
    }
    else if(two == Face::R){
        permute_ri(one);
        permute_ri(three);
    }
    else if(three == Face::R) {
        permute_ri(one);
        permute_ri(two);
    }
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
