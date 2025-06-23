#pragma once

#include "scene/scene_state.hh"
#include "scene/scene3D.hh"

#include "scene2D/manager_2D.hh"


class Manager3D
{
    bool init_flag = false;

    SID sid_3D_counter = 2; // Global index count
    SID new_sid_3D();

public:

    SceneState state;

    Scene3D root_scene;
    Scene3D* window_scene = nullptr;

    std::list<Scene3D> scenes3D; // list of non-root 3D scenes

    /** Initializes manager and creates the root scene. The root scene is returned. */
    Scene3D& init();

};

