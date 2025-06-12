
#pragma once

#include "lib/pair.hh"

#include "ui/render/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/quadS2D.hh"

#include "widget.hh"


namespace UI 
{
namespace W 
{



/** Widget reflecting the state of a specific quad */
struct Quad2DLarge : public Widget
{
    size_t quad_id;
    scene::QuadS2D* quad;
    f2 size = {180.0f, 100.0f};

    BaseString quad_name;
    
    BaseString quad_pos_label;
    BaseString quad_pos_x;
    BaseString quad_pos_y;

public:


    inline EventResult event_handler(window::InputEvent& event)
    {
        using namespace window;

        switch (event.type)
        {

        case EventType::MouseButton:
            if(event.mouse_button.is_left_down())
            {
                if(quad_name.containsPoint(cursor_sane))
                {
                    auto& root_scene = ManagerScene::get_root_scene_mut();
                    root_scene.quad_manager.set_selected(quad_id);
                }
                

                // println("WidgetQuad mouse down!");
                

                // return EventResult::Grab;
            }
            else if(event.mouse_button.is_left_up())
            {
                // println("WidgetQuad mouse up!");
                // return EventResult::Release;
            }
            break;
        
        case EventType::MouseScroll:
        
            if(quad_pos_x.containsPoint(cursor_sane))
            {
                auto& root_scene = ManagerScene::get_root_scene_mut();
                scene::QuadS2D* quad = root_scene.quad_manager.get_quad_mut(quad_id);
                if(quad != nullptr)
                {
                    Box2D updated_box = quad->get_box();

                    if(event.mouse_scroll.delta > 0.0f)
                        updated_box.pos.x += 10.0f;
                    else
                        updated_box.pos.x -= 10.0f;
                    
                    quad->set_box(updated_box.pos, updated_box.size);
                }
            }
            break;

        default:
            break;

        }

        return {};
    }

    /** Recreates the whole widget from scene data every call. */
    inline void reload(scene::QuadS2D* _quad, f2 new_pos)
    {
        // Update quad
        quad = _quad;

        // Frame
        frame.pos = new_pos;
        frame.size = size;
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x444444ff);


        if(quad == nullptr)
            return;

        quad_id = quad->get_id();

        f2 offset = frame.pos;
        offset += {5.0f, frame.size.y};

        // Name
        f2 name_delta = { 0.0f, -20.0f };
        Str name_str = quad->get_name();
        quad_name.set_pos(offset += name_delta);
        quad_name.set_str(name_str);

        // QUAD BOX 
        Box2D box = quad->get_box();
        
        f2 box_indent = { 10.0f, 0.0f };
        offset += box_indent;
        
        // POS

        // label
        f2 pos_label_delta = { 10.0f, -20.0f };
        quad_pos_label.set_pos(offset + pos_label_delta);
        quad_pos_label.set_str("Pos: ");
        // x
        f2 pos_x_delta = { 70.0f, -20.0f };
        quad_pos_x.set_pos(offset + pos_x_delta);
        quad_pos_x.set_str(Str(box.pos.x, 1));
        // y
        f2 pos_y_delta = { 120.0f, -20.0f };
        quad_pos_y.set_pos(offset + pos_y_delta);
        quad_pos_y.set_str(Str(box.pos.y, 1));

        offset -= box_indent;
    }


    inline void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(quad_name);

        renderer.draw_base_string(quad_pos_x);
        renderer.draw_base_string(quad_pos_y);
        renderer.draw_base_string(quad_pos_label);
    }

};



}
}