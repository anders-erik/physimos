
#pragma once

#include "lib/pair.hh"

#include "ui/render/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/quadS2D.hh"


namespace UI {

/** Stateless widget -- constructs the widget, renders, and destroys on each render call */
struct WidgetRootScene {

    f2 cursor_sane; // cursor position at last successful cursor query
    Base* base_hover_target = nullptr; // currently hovered base
    Base* base_grab_target = nullptr; // currently grabbed base
    Pair<size_t, Base*> hover_target = {0, nullptr}; // currently hovered base w id
    Pair<size_t, Base*> grab_target = {0, nullptr}; // currently grabbed base w id

    Box2D frame = {
        {10.0f, 300.0f},
        {200.0f, 200.0f}
    };
    Base frame_base;
    BaseString scene_name_base;

    std::vector<BaseString> quad_base_strings;

public:

    WidgetRootScene() {}

    /** Recreates the whole widget from scene data every call. */
    void reload()
    {
        scene::Scene2D* root_scene = ManagerScene::get_root_scene();

        // Frame
        frame_base.set_box(frame);

        // Start top left
        f2 pos = {frame.pos.x, frame.pos.y + frame.size.y};

        // Name
        f2 name_delta = { 0.0f, -20.0f };
        Str name_str = root_scene->name;
        scene_name_base.set_pos( pos += name_delta );
        // scene_name_base.set_size( offset );
        scene_name_base.set_str(name_str);

        // Quads
        quad_base_strings.clear();
        f2 quad_delta = { 0.0f, -20.0f };
        f2 quad_indent = {10.0f, 0.0f};
        
        pos += quad_indent;
        for(size_t quad_id : root_scene->quad_ids)
        {
            auto* quad = root_scene->quad_manager.get_quad(quad_id);
            if(quad == nullptr)
                continue;

            Str& name_str = quad->get_name();
            BaseString& name_base = quad_base_strings.emplace_back();
            name_base.set_pos(pos += quad_delta);
            name_base.set_size( {20.0f, 10.0f} );
            name_base.set_str(name_str);
        }
        pos -= quad_indent;
        
    }


    /** Resturn base matching set cursor pos or nullptr on no match */
    UI::Base* try_find_hovered_base()
    {
        for(UI::BaseString& quad_string : quad_base_strings)
        {
            if(quad_string.containsPoint(cursor_sane))
            {
                quad_string.set_hover();
                return &quad_string;
            }
        }
        return nullptr;
    }

    /** 
        Check is point is within widget box or is currently grabbing cursor. 
        Stores the cursor position for events. */
    bool has_cursor(f2 cursor_sane)
    {
        if(base_grab_target != nullptr)
        {
            // base_grab_target->click();
            return true;
        }
        
        if(!frame.contains_point(cursor_sane))
            return false;

        this->cursor_sane = cursor_sane;

        // Try hover quad string
        base_hover_target = nullptr; // reset for each frame, unless already grabbing
        base_hover_target = try_find_hovered_base();
        if(base_hover_target != nullptr)
        {
            base_hover_target->set_hover();
        }
        return true;
    }

    void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(scene_name_base);

        for(auto& quad_string : quad_base_strings)
            renderer.draw_base_string(quad_string);
    }


    void scroll(float delta){
    
        println("Widget scroll");

        scene_name_base.scroll(delta);

    }

    void grab()
    {
        
        if(base_hover_target != nullptr)
        {
            base_hover_target->click();
            base_grab_target = base_hover_target;
            println("quad string grab");
        }
    }

    void release()
    {
        // println("Widget release");
        if(base_grab_target != nullptr)
        {
            base_grab_target->unclick(); //
            base_grab_target = nullptr;
            println("quad string release");
        }
    }

};



}