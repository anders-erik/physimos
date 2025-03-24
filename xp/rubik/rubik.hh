
#pragma once

#include <vector>
#include <array>
#include "math.hh"

#include "gpu.hh"
#include "model.hh"
#include "window.hh"



namespace xprubik {





enum class Permutation {
    F,
    Fi,
    B,
    Bi,
    R,
    Ri,
    L,
    Li,
    U,
    Ui,
    D,
    Di,
};

enum class Face {
    None,
    F,
    B,
    R,
    L,
    U,
    D,
};

enum class CubieType {
    Center,
    Edge,
    Corner,
};

struct Faces {
    Face one   = Face::None;
    Face two   = Face::None;
    Face three = Face::None;

    bool contains(Face face);

    void permute_f(Face& face);
    void permute_fi(Face& face);
    void permute_b(Face& face);
    void permute_bi(Face& face);
    void permute_r(Face& face);
    void permute_ri(Face& face);
    void permute_l(Face& face);
    void permute_li(Face& face);
    void permute_u(Face& face);
    void permute_ui(Face& face);
    void permute_d(Face& face);
    void permute_di(Face& face);

    void permute_f_edge(Permutation p);
    void permute_fi_edge(Permutation p);
    void permute_b_edge(Permutation p);
    void permute_bi_edge(Permutation p);
    void permute_r_edge(Permutation p);
    void permute_ri_edge(Permutation p);
    void permute_l_edge(Permutation p);
    void permute_li_edge(Permutation p);
    void permute_u_edge(Permutation p);
    void permute_ui_edge(Permutation p);
    void permute_d_edge(Permutation p);
    void permute_di_edge(Permutation p);

    void permute_f_corner(Permutation p);
    void permute_fi_corner(Permutation p);
    void permute_b_corner(Permutation p);
    void permute_bi_corner(Permutation p);
    void permute_r_corner(Permutation p);
    void permute_ri_corner(Permutation p);
    void permute_l_corner(Permutation p);
    void permute_li_corner(Permutation p);
    void permute_u_corner(Permutation p);
    void permute_ui_corner(Permutation p);
    void permute_d_corner(Permutation p);
    void permute_di_corner(Permutation p);
    
};


struct Animator {
    /** Currently animated permutation. */
    Permutation permutation;

    /** Animation flag. */
    bool is_animating = false;
    /** Synchronize current internal state once when animation is complete. */
    bool synchronization_frame = false;

    /** Always animate 90 deg */
    float animation_angle = 1.5707825f;
    /** Manually step size for 60 frames per anim */
    // float animation_angle_step = 0.026179708f;
    /** Manually step size for 30 frames per anim */
    // float animation_angle_step = 0.053f;
    /** Manually step size for 10 frames per anim */
    float animation_angle_step = 0.15f;


    int frames_per_anim = 10;
    int current_frame_count = 0;
};


/** Keeps track of Cubie rotations relative original rotation. Extrinsic rotations. */
struct CubieRotation {
    m4f4 matrix;
    /** Permute the rotation matrix by 90deg by premultiplying current rotation matrix by rotation matrix of passed axis. */
    void rot(Axis axis);
};


struct Cubie {
    CubieType type;
    CubieRotation c_rot;
    Faces faces;

    bool is_rotating = false; // Animation flag
    
    Model model;

    void set_position_from_faces();
};


/** A Rubik's Cube */
struct Cube {
    std::array<Cubie, 26> cubies;

    /** Performs incremental cubie model rotations to display permutation transitions. */
    Animator animator;

    void permute(Permutation p);

    void update_cubies();

    void update_animator();
    void handle_input(InputState input_state);
    /** Simple prints as we currently don't have a ui */
    void print();

    Cube();
};


}