
#pragma once

#include <vector>
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


enum class Rot90 {
    A0,
    A90,
    A180,
    A270,
};

struct CubieRotation {
    Rot90 x = Rot90::A0;
    Rot90 y = Rot90::A0;
    Rot90 z = Rot90::A0;

    void pos_x();
    void neg_x();

    float get_x_rad();
};

struct Cubie {
    CubieRotation c_rot;
    Model model;
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

/** A Rubik's Cube */
struct Cube {
    Cubie c0;
    Cubie c1;

    /** Performs incremental cubie model rotations to display permutation transitions. */
    Animator animator;


    void update_animator();
    void handle_input(InputState input_state);
    /** Simple prints as we currently don't have a ui */
    void print();

    Cube();
};


}