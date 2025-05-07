
#pragma once

// #include "GLFW/glfw3.h"


struct GLFWwindow; // bypass glfw inclusion


namespace window {


enum class KeyModifier :int {
    alt_ctl_shift   = 7,
    alt_ctrl        = 6,
    alt_shift       = 5,
    alt             = 4,
    ctl_shift       = 3,
    ctl             = 2,
    shift           = 1,
    none            = 0,
};

enum class Key {
    None,
    Esc,
    p,
};


Key get_key_from_glfw_key(int _glfw_key);


}

