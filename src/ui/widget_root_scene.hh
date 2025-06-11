
#pragma once

#include "lib/pair.hh"

#include "ui/render/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/quadS2D.hh"

#include "widget.hh"
#include "widget_quad.hh"

namespace UI {

/** Lists root scene objects */
struct WidgetRootScene : public Widget {


    using quad_base_pair = Pair<size_t, Base*>;
    // quad_base_pair hover_target = {0, nullptr}; // currently hovered base w id
    // quad_base_pair grab_target = {0, nullptr}; // currently grabbed base w id

    // Box2D frame = {
    //     {10.0f, 300.0f},
    //     {200.0f, 200.0f}
    // };
    // Base frame_base;

    // std::vector<Base> bases;

    // std::vector<BaseString> strings;
    std::vector<WidgetQuad> quad_widgets;
    std::vector<Pair<size_t, BaseString>> quad_pairs;

    BaseString scene_name_base;

public:

    WidgetRootScene() = default;


    // /** Resturn base matching set cursor pos or nullptr on no match */
    // UI::Base* try_find_hovered_base()
    // {
    //     for(UI::BaseString& quad_string : strings)
    //     {
    //         if(quad_string.containsPoint(cursor_sane))
    //         {
    //             quad_string.set_hover();
    //             return &quad_string;
    //         }
    //     }
    //     return nullptr;
    // }

    // /** 
    //     Check is point is within widget box or is currently grabbing cursor. 
    //     Stores the cursor position for events. */
    // bool has_cursor(f2 cursor_sane)
    // {
    //     if(grab_target.YY != nullptr)
    //     {
    //         // base_grab_target->click();
    //         return true;
    //     }
        
    //     if(!frame.contains_point(cursor_sane))
    //         return false;

    //     this->cursor_sane = cursor_sane;

    //     // Try hover quad string
    //     hover_target= {0, nullptr}; // reset for each frame, unless already grabbing
    //     hover_target.YY = try_find_hovered_base();
    //     if(hover_target.YY != nullptr)
    //     {
    //         // hover_target.XX = hover_target.YY
    //         hover_target.YY->set_hover();
    //     }
    //     return true;
    // }


    // void scroll(float delta){
    
    //     println("Widget scroll");

    //     scene_name_base.scroll(delta);

    // }

    // void mouse_left_down()
    // {
    //     if(hover_target.YY != nullptr)
    //     {
    //         hover_target.YY->click();
    //         grab_target = hover_target;
    //         println("quad string grab");
    //         auto* root_scene = ManagerScene::get_root_scene();
    //         root_scene->quad_manager.set_selected(hover_target.XX);
    //     }
    // }

    // void mouse_left_up()
    // {
    //     // println("Widget release");
    //     if(grab_target.YY != nullptr)
    //     {
    //         grab_target.YY->unclick();
    //         grab_target = {0, nullptr};
    //         println("quad string release");
    //     }
    // }


    /** Resturn base matching set cursor pos or nullptr on no match */
    size_t try_find_quad_id_at_cursor_location()
    {
        for(auto& quad_pair : quad_pairs)
        {
            if(quad_pair.YY.containsPoint(cursor_sane))
            {
                // quad_string.YY.set_hover();
                return quad_pair.XX;
            }
        }
        return 0;
    }

    EventResult event_handler(window::InputEvent& event)
    {

        for(auto& quad_widget : quad_widgets)
        {
            if(quad_widget.has_cursor(cursor_sane))
            {
                quad_widget.event_handler(event);
            }
        }

        if(event.is_mouse_button())
        {
            if(event.mouse_button.is_left_down())
            {
                println("RootScene widget mouse down!");
                size_t quad_id = try_find_quad_id_at_cursor_location();
                if(quad_id != 0)
                {
                    auto& root_scene = ManagerScene::get_root_scene_mut();
                    root_scene.quad_manager.set_selected(quad_id);
                }
                return EventResult::Grab;
            }
            else if(event.mouse_button.is_left_up())
            {
                println("RootScene widget mouse up!");
                return EventResult::Release;
            }
        }
        return {};
    }



    /** Recreates the whole widget from scene data every call. */
    void reload(f2 widget_pos)
    {
        scene::Scene2D& root_scene = ManagerScene::get_root_scene_mut();
        

        // Frame
        frame.pos = widget_pos;
        frame.size = {200.0f, 200.0f};
        frame_base.set_box(frame);

        // Start top left
        f2 pos = {frame.pos.x, frame.pos.y + frame.size.y};

        // Name
        f2 name_delta = { 0.0f, -20.0f };
        Str name_str = root_scene.get_name();
        scene_name_base.set_pos( pos += name_delta );
        // scene_name_base.set_size( offset );
        scene_name_base.set_str(name_str);


        // Quad Widgets
        quad_widgets.clear();

        f2 quad_w_delta = { 0.0f, -32.0f };
        f2 quad_indent = {10.0f, 0.0f};
        
        pos += quad_indent;
        for(auto& quad : root_scene.quad_manager.get_quads_mut())
        {
            WidgetQuad quad_widget;
            quad_widget.reload(&quad, pos += quad_w_delta);
            quad_widgets.push_back(quad_widget);
        }
        pos -= quad_indent;
    }


    void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(scene_name_base);

        for(auto& quad_string : quad_pairs)
            renderer.draw_base_string(quad_string.YY);

        for(auto& quad_widget : quad_widgets)
            quad_widget.render(renderer);
    }
};



}