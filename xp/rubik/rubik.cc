
#include "math.hh"
#include "model.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "rubik.hh"

namespace xprubik {




Cube::Cube() {


    f3 red =   f3(1.0f, 0.0f, 0.0f);
    f3 green = f3(0.0f, 1.0f, 0.0f);
    f3 blue =  f3(0.0f, 0.0f, 1.0f);
    f3 orange = f3(1.0f, 0.5f, 0.0f);
    f3 yellow = f3(1.0f, 1.0f, 0.0f);
    f3 white =  f3(1.0f, 1.0f, 1.0f);

    f3 f_color = green;
    f3 b_color = blue;
    f3 r_color = red;
    f3 l_color = orange;
    f3 u_color = yellow;
    f3 d_color = white;
    // f3 f_color = white;
    // f3 b_color = yellow;
    // f3 r_color = red;
    // f3 l_color = orange;
    // f3 u_color = green;
    // f3 d_color = blue;


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

    Cubie& b = cubies[15];
    Cubie& bl = cubies[16];
    Cubie& bu = cubies[17];
    Cubie& bd = cubies[18];
    Cubie& blu = cubies[19];
    Cubie& bld = cubies[20];

    Cubie& l = cubies[21];
    Cubie& lu = cubies[22];
    Cubie& ld = cubies[23];

    Cubie& u = cubies[24];

    Cubie& d = cubies[25];
    

    // F 
    f.set_center_face(Face::F);
    model_add_cube_mesh(f.model.mesh);
    model_add_facelet(f.model.mesh, f_color, Axis::x);

    fr.set_edge_faces(Face::F, Face::R);
    model_add_cube_mesh(fr.model.mesh);
    model_add_facelet(fr.model.mesh, f_color, Axis::x);
    model_add_facelet(fr.model.mesh, r_color, Axis::y);

    fl.set_edge_faces(Face::F, Face::L);
    model_add_cube_mesh(fl.model.mesh);
    model_add_facelet(fl.model.mesh, f_color, Axis::x);
    model_add_facelet(fl.model.mesh, l_color, Axis::ny);

    fu.set_edge_faces(Face::F, Face::U);
    model_add_cube_mesh(fu.model.mesh);
    model_add_facelet(fu.model.mesh, f_color, Axis::x);
    model_add_facelet(fu.model.mesh, u_color, Axis::z);

    fd.set_edge_faces(Face::F, Face::D);
    model_add_cube_mesh(fd.model.mesh);
    model_add_facelet(fd.model.mesh, f_color, Axis::x);
    model_add_facelet(fd.model.mesh, d_color, Axis::nz);

    fru.set_corner_faces(Face::F, Face::R, Face::U);
    model_add_cube_mesh(fru.model.mesh);
    model_add_facelet(fru.model.mesh, f_color, Axis::x);
    model_add_facelet(fru.model.mesh, r_color, Axis::y);
    model_add_facelet(fru.model.mesh, u_color, Axis::z);

    frd.set_corner_faces(Face::F, Face::R, Face::D);
    model_add_cube_mesh(frd.model.mesh);
    model_add_facelet(frd.model.mesh, f_color, Axis::x);
    model_add_facelet(frd.model.mesh, r_color, Axis::y);
    model_add_facelet(frd.model.mesh, d_color, Axis::nz);

    flu.set_corner_faces(Face::F, Face::L, Face::U);
    model_add_cube_mesh(flu.model.mesh);
    model_add_facelet(flu.model.mesh, f_color, Axis::x);
    model_add_facelet(flu.model.mesh, l_color, Axis::ny);
    model_add_facelet(flu.model.mesh, u_color, Axis::z);

    fld.set_corner_faces(Face::F, Face::L, Face::D);
    model_add_cube_mesh(fld.model.mesh);
    model_add_facelet(fld.model.mesh, f_color, Axis::x);
    model_add_facelet(fld.model.mesh, l_color, Axis::ny);
    model_add_facelet(fld.model.mesh, d_color, Axis::nz);



    // R
    r.set_center_face(Face::R);
    model_add_cube_mesh(r.model.mesh);
    model_add_facelet(r.model.mesh, r_color, Axis::y);

    rb.set_edge_faces(Face::R, Face::B);
    model_add_cube_mesh(rb.model.mesh);
    model_add_facelet(rb.model.mesh, r_color, Axis::y);
    model_add_facelet(rb.model.mesh, b_color, Axis::nx);

    ru.set_edge_faces(Face::R, Face::U);
    model_add_cube_mesh(ru.model.mesh);
    model_add_facelet(ru.model.mesh, r_color, Axis::y);
    model_add_facelet(ru.model.mesh, u_color, Axis::z);

    rd.set_edge_faces(Face::R, Face::D);
    model_add_cube_mesh(rd.model.mesh);
    model_add_facelet(rd.model.mesh, r_color, Axis::y);
    model_add_facelet(rd.model.mesh, d_color, Axis::nz);

    rbu.set_corner_faces(Face::R, Face::B, Face::U);
    model_add_cube_mesh(rbu.model.mesh);
    model_add_facelet(rbu.model.mesh, r_color, Axis::y);
    model_add_facelet(rbu.model.mesh, b_color, Axis::nx);
    model_add_facelet(rbu.model.mesh, u_color, Axis::z);

    rbd.set_corner_faces(Face::R, Face::B, Face::D);
    model_add_cube_mesh(rbd.model.mesh);
    model_add_facelet(rbd.model.mesh, r_color, Axis::y);
    model_add_facelet(rbd.model.mesh, b_color, Axis::nx);
    model_add_facelet(rbd.model.mesh, d_color, Axis::nz);





    // B
    b.set_center_face(Face::B);
    model_add_cube_mesh(b.model.mesh);
    model_add_facelet(b.model.mesh, b_color, Axis::nx);

    bl.set_edge_faces(Face::B, Face::L);
    model_add_cube_mesh(bl.model.mesh);
    model_add_facelet(bl.model.mesh, b_color, Axis::nx);
    model_add_facelet(bl.model.mesh, l_color, Axis::ny);

    bu.set_edge_faces(Face::B, Face::U);
    model_add_cube_mesh(bu.model.mesh);
    model_add_facelet(bu.model.mesh, b_color, Axis::nx);
    model_add_facelet(bu.model.mesh, u_color, Axis::z);

    bd.set_edge_faces(Face::B, Face::D);
    model_add_cube_mesh(bd.model.mesh);
    model_add_facelet(bd.model.mesh, b_color, Axis::nx);
    model_add_facelet(bd.model.mesh, d_color, Axis::nz);

    blu.set_corner_faces(Face::B, Face::L, Face::U);
    model_add_cube_mesh(blu.model.mesh);
    model_add_facelet(blu.model.mesh, b_color, Axis::nx);
    model_add_facelet(blu.model.mesh, l_color, Axis::ny);
    model_add_facelet(blu.model.mesh, u_color, Axis::z);

    bld.set_corner_faces(Face::B, Face::L, Face::D);
    model_add_cube_mesh(bld.model.mesh);
    model_add_facelet(bld.model.mesh, b_color, Axis::nx);
    model_add_facelet(bld.model.mesh, l_color, Axis::ny);
    model_add_facelet(bld.model.mesh, d_color, Axis::nz);


    // L
    l.set_center_face(Face::L);
    model_add_cube_mesh(l.model.mesh);
    model_add_facelet(l.model.mesh, l_color, Axis::ny);

    lu.set_edge_faces(Face::L, Face::U);
    model_add_cube_mesh(lu.model.mesh);
    model_add_facelet(lu.model.mesh, l_color, Axis::ny);
    model_add_facelet(lu.model.mesh, u_color, Axis::z);

    ld.set_edge_faces(Face::L, Face::D);
    model_add_cube_mesh(ld.model.mesh);
    model_add_facelet(ld.model.mesh, l_color, Axis::ny);
    model_add_facelet(ld.model.mesh, d_color, Axis::nz);



    // U
    u.set_center_face(Face::U);
    model_add_cube_mesh(u.model.mesh);
    model_add_facelet(u.model.mesh, u_color, Axis::z);


    // D
    d.set_center_face(Face::D);
    model_add_cube_mesh(d.model.mesh);
    model_add_facelet(d.model.mesh, d_color, Axis::nz);


};

void Cube::solve(){

    Cubie fb = find_edge_home(Face::F, Face::D);

    

}

Cubie Cube::find_edge_home(Face face_1, Face face_2){
    Cubie return_cubie;
    for(Cubie& c : cubies){
        
        if(c.type == CubieType::Edge){
        
            if(c.contains_face_home(face_1) && c.contains_face_home(face_2))
                return c;
        
        }

    }

    return return_cubie;
}

void Cube::permute(std::string permutation_string){

    for(char c : permutation_string){
    
        switch (c)
        {

        case 'F':
            permutation_backlog.push_back(Permutation::F);
            break;
        case 'f':
            permutation_backlog.push_back(Permutation::Fi);
            break;
        case 'B':
            permutation_backlog.push_back(Permutation::B);
            break;
        case 'b':
            permutation_backlog.push_back(Permutation::Bi);
            break;
        case 'R':
            permutation_backlog.push_back(Permutation::R);
            break;
        case 'r':
            permutation_backlog.push_back(Permutation::Ri);
            break;
        case 'L':
            permutation_backlog.push_back(Permutation::L);
            break;
        case 'l':
            permutation_backlog.push_back(Permutation::Li);
            break;
        case 'U':
            permutation_backlog.push_back(Permutation::U);
            break;
        case 'u':
            permutation_backlog.push_back(Permutation::Ui);
            break;
        case 'D':
            permutation_backlog.push_back(Permutation::D);
            break;
        case 'd':
            permutation_backlog.push_back(Permutation::Di);
            break;
        
        
        default:
            break;
        }
    
    }

}

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

            case Permutation::B:
                if(!c.faces.contains(Face::B))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_b_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_b_corner(permutation);

                c.c_rot.rot(Axis::x);
                break;

            case Permutation::Bi:
                if(!c.faces.contains(Face::B))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_bi_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_bi_corner(permutation);

                c.c_rot.rot(Axis::nx);
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

            case Permutation::L:
                if(!c.faces.contains(Face::L))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_l_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_l_corner(permutation);

                c.c_rot.rot(Axis::y);
                break;

            case Permutation::Li:
                if(!c.faces.contains(Face::L))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_li_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_li_corner(permutation);

                c.c_rot.rot(Axis::ny);

                break;


            case Permutation::U:
                if(!c.faces.contains(Face::U))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_u_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_u_corner(permutation);

                c.c_rot.rot(Axis::nz);
                break;

            case Permutation::Ui:
                if(!c.faces.contains(Face::U))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_ui_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_ui_corner(permutation);

                c.c_rot.rot(Axis::z);

                break;

            case Permutation::D:
                if(!c.faces.contains(Face::D))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_d_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_d_corner(permutation);

                c.c_rot.rot(Axis::z);
                break;

            case Permutation::Di:
                if(!c.faces.contains(Face::D))
                    continue;

                if(c.type == CubieType::Edge)
                    c.faces.permute_di_edge(permutation);

                if(c.type == CubieType::Corner)
                    c.faces.permute_di_corner(permutation);

                c.c_rot.rot(Axis::nz);

                break;
        
        default:
            break;
        }

        c.is_rotating = true;
    }

}

bool Cubie::contains_face_home(Face face){
    if      (faces_home.one == face)
        return true;
    else if (faces_home.two == face)
        return true;
    else if (faces_home.three == face)
        return true;

    return false;
}

void Cubie::set_center_face(Face face_1){
    type = CubieType::Center;

    faces.one       = face_1;
    faces_home.one  = face_1;
}
void Cubie::set_edge_faces(Face face_1, Face face_2){
    type = CubieType::Edge;

    faces.one       = face_1;
    faces_home.one  = face_1;
    
    faces.two       = face_2;
    faces_home.two  = face_2;
}
void Cubie::set_corner_faces(Face face_1, Face face_2, Face face_3){
    type = CubieType::Corner;

    faces.one       = face_1;
    faces_home.one  = face_1;

    faces.two       = face_2;
    faces_home.two  = face_2;

    faces.three     = face_3;
    faces_home.three = face_3;
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
        animator.synchronization_frame = false;

        for(Cubie& c : cubies){

            // Reset
            c.model.transform.pos = f3();
            c.model.transform.rot = f3();

            // Update pos and rot vectors
            c.set_position_from_faces();

            c.model.set_transform_matrix();

            mat_mul(c.model.transform.matrix, c.c_rot.matrix);
        }


        // After synchronization, we check backlog
        if(permutation_backlog.size() > 0){
            permute(permutation_backlog.back());
            permutation_backlog.pop_back();
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
                
                    case Permutation::B :
                        rot_mat.rotate_x(animator.animation_angle_step);
                        break;

                    case Permutation::Bi :
                        rot_mat.rotate_x(-animator.animation_angle_step);
                        break;

                    case Permutation::R :
                        rot_mat.rotate_y(-animator.animation_angle_step);
                        break;

                    case Permutation::Ri :
                        rot_mat.rotate_y(animator.animation_angle_step);
                        break;
                
                    case Permutation::L :
                        rot_mat.rotate_y(animator.animation_angle_step);
                        break;

                    case Permutation::Li :
                        rot_mat.rotate_y(-animator.animation_angle_step);
                        break;
                    
                    case Permutation::U :
                        rot_mat.rotate_z(animator.animation_angle_step);
                        break;

                    case Permutation::Ui :
                        rot_mat.rotate_z(-animator.animation_angle_step);
                        break;
                
                    case Permutation::D :
                        rot_mat.rotate_z(-animator.animation_angle_step);
                        break;

                    case Permutation::Di :
                        rot_mat.rotate_z(animator.animation_angle_step);
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

    if(animator.is_animating || animator.synchronization_frame)
        return;

    if(input_state.up)
        permute("FFFFFFFF");

    if(input_state.m){ // mix
        permute("FUBlrubRLDfd");
    }
    if(input_state.s){ // solve
        solve();
    }

    if(input_state.f && input_state.shift)
        permute(Permutation::Fi);
    else if(input_state.f)
        permute(Permutation::F);
    else if(input_state.b && input_state.shift)
        permute(Permutation::Bi);
    else if(input_state.b)
        permute(Permutation::B);

    else if(input_state.r && input_state.shift)
        permute(Permutation::Ri);
    else if(input_state.r)
        permute(Permutation::R);
    else if(input_state.l && input_state.shift)
        permute(Permutation::Li);
    else if(input_state.l)
        permute(Permutation::L);

    else if(input_state.u && input_state.shift)
        permute(Permutation::Ui);
    else if(input_state.u)
        permute(Permutation::U);
    else if(input_state.d && input_state.shift)
        permute(Permutation::Di);
    else if(input_state.d)
        permute(Permutation::D);
    



    if(input_state.p){
        print();
    }


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
        
        animator.synchronization_frame = true;
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

void Faces::permute_b(Face& face){
    if(face == Face::R)
        face = Face::U;
    else if(face == Face::D)
        face = Face::R;
    else if(face == Face::L)
        face = Face::D;
    else if(face == Face::U)
        face = Face::L;
}
void Faces::permute_bi(Face& face){
    if(face == Face::R)
        face = Face::D;
    else if(face == Face::D)
        face = Face::L;
    else if(face == Face::L)
        face = Face::U;
    else if(face == Face::U)
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

void Faces::permute_l(Face& face){
    if(face == Face::F)
        face = Face::D;
    else if(face == Face::U)
        face = Face::F;
    else if(face == Face::B)
        face = Face::U;
    else if(face == Face::D)
        face = Face::B;
}
void Faces::permute_li(Face& face){
    if(face == Face::F)
        face = Face::U;
    else if(face == Face::U)
        face = Face::B;
    else if(face == Face::B)
        face = Face::D;
    else if(face == Face::D)
        face = Face::F;
}
void Faces::permute_u(Face& face){
    if(face == Face::F)
        face = Face::L;
    else if(face == Face::R)
        face = Face::F;
    else if(face == Face::B)
        face = Face::R;
    else if(face == Face::L)
        face = Face::B;
}
void Faces::permute_ui(Face& face){
    if(face == Face::F)
        face = Face::R;
    else if(face == Face::R)
        face = Face::B;
    else if(face == Face::B)
        face = Face::L;
    else if(face == Face::L)
        face = Face::F;
}
void Faces::permute_d(Face& face){
    if(face == Face::F)
        face = Face::R;
    else if(face == Face::R)
        face = Face::B;
    else if(face == Face::B)
        face = Face::L;
    else if(face == Face::L)
        face = Face::F;
}
void Faces::permute_di(Face& face){
    if(face == Face::F)
        face = Face::L;
    else if(face == Face::R)
        face = Face::F;
    else if(face == Face::B)
        face = Face::R;
    else if(face == Face::L)
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
void Faces::permute_b_edge(Permutation p){
    Face& non_b = (one == Face::B) ? two : one;
    permute_b(non_b);
}
void Faces::permute_bi_edge(Permutation p){
    Face& non_b = (one == Face::B) ? two : one;
    permute_bi(non_b);
}
void Faces::permute_r_edge(Permutation p){
    Face& non_r = (one == Face::R) ? two : one;
    permute_r(non_r);
}
void Faces::permute_ri_edge(Permutation p){
    Face& non_r = (one == Face::R) ? two : one;
    permute_ri(non_r);
}
void Faces::permute_l_edge(Permutation p){
    Face& non_l = (one == Face::L) ? two : one;
    permute_l(non_l);
}
void Faces::permute_li_edge(Permutation p){
    Face& non_l = (one == Face::L) ? two : one;
    permute_li(non_l);
}
void Faces::permute_u_edge(Permutation p){
    Face& non_u = (one == Face::U) ? two : one;
    permute_u(non_u);
}
void Faces::permute_ui_edge(Permutation p){
    Face& non_u = (one == Face::U) ? two : one;
    permute_ui(non_u);
}
void Faces::permute_d_edge(Permutation p){
    Face& non_d = (one == Face::D) ? two : one;
    permute_d(non_d);
}
void Faces::permute_di_edge(Permutation p){
    Face& non_d = (one == Face::D) ? two : one;
    permute_di(non_d);
}

void Faces::permute_f_corner(Permutation p){

    Face face_static = Face::F;

    if(one == face_static){
        permute_f(two);
        permute_f(three);
    }
    else if(two == face_static){
        permute_f(one);
        permute_f(three);
    }
    else if(three == face_static) {
        permute_f(one);
        permute_f(two);
    }

}
void Faces::permute_fi_corner(Permutation p){
    Face face_static = Face::F;

    if(one == face_static){
        permute_fi(two);
        permute_fi(three);
    }
    else if(two == face_static){
        permute_fi(one);
        permute_fi(three);
    }
    else if(three == face_static) {
        permute_fi(one);
        permute_fi(two);
    }
}
void Faces::permute_b_corner(Permutation p){
    Face face_static = Face::B;

    if(one == face_static){
        permute_b(two);
        permute_b(three);
    }
    else if(two == face_static){
        permute_b(one);
        permute_b(three);
    }
    else if(three == face_static) {
        permute_b(one);
        permute_b(two);
    }
    
}
void Faces::permute_bi_corner(Permutation p){
    Face face_static = Face::B;

    if(one == face_static){
        permute_bi(two);
        permute_bi(three);
    }
    else if(two == face_static){
        permute_bi(one);
        permute_bi(three);
    }
    else if(three == face_static) {
        permute_bi(one);
        permute_bi(two);
    }
}
void Faces::permute_r_corner(Permutation p){
    Face face_static = Face::R;

    if(one == face_static){
        permute_r(two);
        permute_r(three);
    }
    else if(two == face_static){
        permute_r(one);
        permute_r(three);
    }
    else if(three == face_static) {
        permute_r(one);
        permute_r(two);
    }
}
void Faces::permute_ri_corner(Permutation p){
    Face face_static = Face::R;

    if(one == face_static){
        permute_ri(two);
        permute_ri(three);
    }
    else if(two == face_static){
        permute_ri(one);
        permute_ri(three);
    }
    else if(three == face_static) {
        permute_ri(one);
        permute_ri(two);
    }
}
void Faces::permute_l_corner(Permutation p){
    Face face_static = Face::L;

    if(one == face_static){
        permute_l(two);
        permute_l(three);
    }
    else if(two == face_static){
        permute_l(one);
        permute_l(three);
    }
    else if(three == face_static) {
        permute_l(one);
        permute_l(two);
    }
}
void Faces::permute_li_corner(Permutation p){
    Face face_static = Face::L;

    if(one == face_static){
        permute_li(two);
        permute_li(three);
    }
    else if(two == face_static){
        permute_li(one);
        permute_li(three);
    }
    else if(three == face_static) {
        permute_li(one);
        permute_li(two);
    }
}
void Faces::permute_u_corner(Permutation p){
    Face face_static = Face::U;

    if(one == face_static){
        permute_u(two);
        permute_u(three);
    }
    else if(two == face_static){
        permute_u(one);
        permute_u(three);
    }
    else if(three == face_static) {
        permute_u(one);
        permute_u(two);
    }
}
void Faces::permute_ui_corner(Permutation p){
    Face face_static = Face::U;

    if(one == face_static){
        permute_ui(two);
        permute_ui(three);
    }
    else if(two == face_static){
        permute_ui(one);
        permute_ui(three);
    }
    else if(three == face_static) {
        permute_ui(one);
        permute_ui(two);
    }
}
void Faces::permute_d_corner(Permutation p){
    Face face_static = Face::D;

    if(one == face_static){
        permute_d(two);
        permute_d(three);
    }
    else if(two == face_static){
        permute_d(one);
        permute_d(three);
    }
    else if(three == face_static) {
        permute_d(one);
        permute_d(two);
    }
}
void Faces::permute_di_corner(Permutation p){
    Face face_static = Face::D;

    if(one == face_static){
        permute_di(two);
        permute_di(three);
    }
    else if(two == face_static){
        permute_di(one);
        permute_di(three);
    }
    else if(three == face_static) {
        permute_di(one);
        permute_di(two);
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
