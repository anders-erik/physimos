
#pragma once

#include "lib/pair.hh"

#include "ui/render/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/quadS2D.hh"

#include "widget.hh"


namespace UI {

/** Widget reflecting the state of a specific quad */
struct WidgetQuad : public Widget
{
    size_t quad_id;
    scene::QuadS2D* quad;
    f2 size = {180.0f, 30.0f};

    BaseString quad_name;

public:

    EventResult event_handler(window::InputEvent& event)
    {
        using namespace window;

        switch (event.type)
        {

        case EventType::MouseButton:
            if(event.mouse_button.is_left_down())
            {
                // println("WidgetQuad mouse down!");
                auto& root_scene = ManagerScene::get_root_scene_mut();
                root_scene.quad_manager.set_selected(quad_id);

                // return EventResult::Grab;
            }
            else if(event.mouse_button.is_left_up())
            {
                // println("WidgetQuad mouse up!");
                // return EventResult::Release;
            }
            break;
        

        default:
            break;

        }

        return {};
    }

    /** Recreates the whole widget from scene data every call. */
    void reload(scene::QuadS2D* _quad, f2 new_pos)
    {
        if(_quad == nullptr)
            return;

        // Update quad
        quad = _quad;
        if(quad != nullptr)
            quad_id = quad->get_id();

        // Frame
        frame.pos = new_pos;
        frame.size = size;
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x444444ff);

        // Name
        f2 name_delta = { 5.0f, 6.0f };
        Str name_str = quad->get_name();
        quad_name.set_pos(frame.pos + name_delta);
        quad_name.set_str(name_str);

    }



    void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(quad_name);

    }

};



}