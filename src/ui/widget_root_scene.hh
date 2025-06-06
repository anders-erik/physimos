
#pragma once

#include "ui/render/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/quadS2D.hh"

namespace UI {

/** Stateless widget -- constructs the widget, renders, and destroys on each render call */
struct WidgetRootScene {


    WidgetRootScene() {}

    void render(RendererBase& renderer){

        scene::Scene2D* root_scene = ManagerScene::get_root_scene();

        f2 pos = {10.0f, 500.0f};

        // Name
        Str name_str = root_scene->name;
        BaseString name_base;
        name_base.set_pos( {10.0f, 500.0f} );
        name_base.set_str(name_str);
        renderer.draw_base_string(name_base);


        // Quads
        float offset = 0.0f;
        float offset_delta = -20.0f;

        for(auto& quad : root_scene->quads){
            offset += offset_delta;

            Str& name_str = quad.get_name();
            BaseString name_base;
            name_base.set_pos( {pos.x+10.0f, pos.y + offset} );
            name_base.set_str(name_str);
            renderer.draw_base_string(name_base);
        }
        
    }

};



}