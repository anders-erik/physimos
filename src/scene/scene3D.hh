
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer_model_texture.hh"
#include "opengl/renderer_vector.hh"
#include "opengl/renderer_model_color.hh"

#include "math/vecmat.hh"

#include "window/auxevent.hh"

#include "camera.hh"

#include "renderer_axes.hh"

#include "sid.hh"

#include "rend/scene3D/program_model_texture.hh"
#include "rend/scene3D/program_model_color.hh"
#include "rend/scene3D/program_axes.hh"

#include "scene/object_manager.hh"
#include "scene/scene_state.hh"


struct Scene3D
{
    SID sid;
    Str name;

    CameraOrbital camera;

    std::vector<TagO> tagos;

    std::vector<model::ModelT> texture_models;
    std::vector<model::ModelColor> color_models;

    model::ModelT tube;

    f3 quarternion_vector_100 = {1.0f, 0.0f, 0.0f};
    f3 quarternion_vector_111 = {0.0f, 0.0f, 1.0f};

};

