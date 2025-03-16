
#pragma once

#include <vector>

#include "gpu.hh"
#include "gpu_renderers.hh"
#include "math.hh"
#include "model.hh"

namespace xprubik {

struct Camera {
    Transform transform;
    
    m4f4 view_mat;
    m4f4 perspective_mat;

    void set_matrices();
};

bool scene_init();

void scene_render();

}