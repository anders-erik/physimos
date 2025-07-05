
#pragma once

#include <vector>

#include "opengl/program.hh"

#include "math/vecmat.hh"

#include "window/auxevent.hh"

#include "camera.hh"


#include "sid.hh"

#include "scene/model.hh"

#include "scene/manager_object.hh"
#include "scene/scene_state.hh"


struct Scene3D
{
    SID sid;
    Str name;

    CameraOrbital camera;
    TagO camera_tag;

    std::vector<TagO> tagos;

    std::vector<model::ModelT> texture_models;
    std::vector<model::ModelColor> color_models;

    model::ModelT tube;

    f3 q_vec_100 = {1.0f, 0.0f, 0.0f};
    f3 q_vec_010 = {0.0f, 1.0f, 0.0f};
    f3 q_vec_001 = {0.0f, 0.0f, 1.0f};

    Quarternion q_1000 = {1.0f, 0.0f, 0.0f, 0.0f};
    f3 q_1000_vec = {1.0f, 0.0f, 0.0f};

};

