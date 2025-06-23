
#pragma once

#include <list>

#include "scene/sid.hh"

#include "scene2D/scene2D.hh"
#include "scene2D/quad_manager.hh"



struct Manager2D
{
    SID sid_2D_counter = 1;

    std::list<Scene2D> scenes2D;
    ManagerQuadS2D manager_quad2d;


    SID new_sid_2D();

    SID push_scene2D(Scene2D& new_scene);

    Scene2D* search_scene_storage_2D(SID id);
    
    [[nodiscard]] std::list<Scene2D>& get_all_scene2D();
};