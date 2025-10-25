
#pragma once

#include <vector>
#include <functional>

#include "lib/arr.hh"

#include "opengl/program.hh"

#include "math/vecmat.hh"

#include "window/auxevent.hh"

#include "scene/camera.hh"


#include "sid.hh"

#include "scene/model.hh"

#include "scene/manager_object.hh"
#include "scene/scene_state.hh"

#include "io/gcode/gcode.hh"


struct SceneProperty
{
    Str name;

};



struct SceneObject
{
    SID sid;
    Str name;
    // Arr<SceneProperty> properties;
    SceneProperty* prop1;
    // SceneProperty prop2;
    // SceneProperty prop3;
};

struct Scene3D
{
    SID sid;
    Str name;

    Camera camera;

    std::vector<TagO> tagos;
    std::vector<TagO> quad_tags;
    std::vector<TagO> lamp_tags;

    std::vector<GCode> g_codes;


    f3 q_vec_100 = {1.0f, 0.0f, 0.0f};
    f3 q_vec_010 = {0.0f, 1.0f, 0.0f};
    f3 q_vec_001 = {0.0f, 0.0f, 1.0f};

    Quarternion q_1000 = {1.0f, 0.0f, 0.0f, 0.0f};
    f3 q_1000_vec = {1.0f, 0.0f, 0.0f};

};

