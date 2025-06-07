
#pragma once

#include "ui/render/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/quadS2D.hh"

namespace UI {

/** Stateless widget -- constructs the widget, renders, and destroys on each render call */
struct WidgetRootScene {

    Box2D box = {
        {10.0f, 500.0f},
        {100.0f, 200.0f}
    };

    BaseString scene_name_base;

    std::vector<BaseString> quad_base_strings;

public:

    WidgetRootScene() {}

    void populate(){

        scene::Scene2D* root_scene = ManagerScene::get_root_scene();

        // Name
        Str name_str = root_scene->name;
        scene_name_base.set_pos( box.pos );
        scene_name_base.set_size( {10.0f, 10.0f} );
        scene_name_base.set_str(name_str);


        // Quads
        quad_base_strings.clear();
        float offset_current = 0.0f;
        float offset_delta = -20.0f;
        float indent_pos = box.pos.x + 10.0f;

        for(size_t quad_id : root_scene->quad_ids)
        {
            auto* quad = root_scene->quad_manager.get_quad(quad_id);
            if(quad == nullptr)
                continue;

            // Add offset after confirmed quad
            offset_current += offset_delta;

            Str& name_str = quad->get_name();
            BaseString& name_base = quad_base_strings.emplace_back();
            name_base.set_pos( {indent_pos, box.pos.y + offset_current} );
            name_base.set_str(name_str);
        }
        
    }

    Base* containsPoint(f2 point){
        if(box.contains_point(point)){

            return &scene_name_base;
        }
        else
        {
            return nullptr;
        }
    }

    void render(RendererBase& renderer)
    {
        renderer.draw_base_string(scene_name_base);

        for(auto& quad_string : quad_base_strings)
            renderer.draw_base_string(quad_string);
    }

};



}