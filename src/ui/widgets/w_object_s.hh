
#pragma once

#include "lib/pair.hh"
#include "lib/print.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"

#include "scene2D/scene2D.hh"
#include "scene2D/quadS2D.hh"

#include "scene/manager_3D.hh"
#include "scene/manager_object.hh"

#include "widget.hh"


namespace UI {
namespace W {

/** Widget reflecting the state of a specific quad */
struct ObjectSmall : public Widget
{
    Object object;
    f2 size = {180.0f, 30.0f};

    BaseString quad_name;

public:

    InputResponse event_handler(Manager3D& manager_3D, window::InputEvent& event)
    {
        using namespace window;

        switch (event.type)
        {

        case EventType::MouseButton:

            if(event.mouse_button.is_left_down())
            {
                println("Clicked on W::ObjectSmall!");

                // Scene3D& scene = ManagerScene::get_window_scene_mut();

                manager_3D.state.selected.try_select(object.tag);

                // auto& q_manager = ManagerScene::get_quad_manager();
                // q_manager.set_selected(quad_id);

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
    void reload(const Object& new_object, f2 new_pos)
    {
        object = new_object;

        // Frame
        frame.pos = new_pos;
        frame.size = size;
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x444444ff);

        // Name
        f2 name_delta = { 5.0f, 6.0f };
        quad_name.set_pos(frame.pos + name_delta);
        quad_name.set_str(object.name);
    }



    void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(quad_name);
    }

};


}
}