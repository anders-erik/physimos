
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

enum class Rot90 {
    A0,
    A90,
    A180,
    A270,
};

struct Faces {
    Face one   = Face::None;
    Face two   = Face::None;
    Face three = Face::None;

    
    void permute_f_edge(Permutation p);
    void permute_fi_edge(Permutation p);
    void permute_r_edge(Permutation p);
    void permute_ri_edge(Permutation p);

    // void permute_f_corner(Permutation p);


    bool contains(Face face);
};


struct Animator {
    /** Currently animated permutation. */
    Permutation permutation;

    bool is_animating = false;

    /** Always animate 90 deg */
    float animation_angle = 1.5707825f;
    /** Manually step size for 60 frames per anim */
    // float animation_angle_step = 0.026179708f;
    /** Manually step size for 30 frames per anim */
    float animation_angle_step = 0.053f;


    int frames_per_anim = 30;
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

    // void update_animation(Permutation perm, float angle_delta);
    void set_position_from_faces();
    // void permute(Permutation permutation);
    // void set_rotation_transform_from_discrete_rot();
};


/** A Rubik's Cube */
struct Cube {
    std::array<Cubie, 8> cubies;
    Cubie c_xp;
    // Cubie c1;

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