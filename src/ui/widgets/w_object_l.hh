
#pragma once

#include "lib/pair.hh"
#include "lib/print.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"

#include "scene/object.hh"
#include "scene2D/scene2D.hh"
#include "scene2D/quadS2D.hh"

#include "scene/manager_3D.hh"
#include "scene/manager_object.hh"

#include "widget.hh"

#include "widgets/w_f2.hh"
#include "widgets/w_f3.hh"
#include "widgets/w_q.hh"

#include "widgets/w_mesh.hh"


namespace UI 
{
namespace W 
{



/** Widget reflecting the state of a specific quad */
struct ObjectLarge : public Widget
{
    Object object; // object copy - read only
    static constexpr float H = 300.0f;


    BaseString object_name;

    BaseString S_orbit;

    W::F3 w_pos;
    W::Q w_rot;

    W::Mesh w_mesh;

public:


    inline InputResponse event_handler(Manager3D& manager_3D, window::InputEvent& event)
    {
        using namespace window;

        if(w_pos.has_cursor(cursor_sane))
        {
            w_pos.event_handler(event);
            return {};
        }

        
        if(S_orbit.box.contains_point(cursor_sane))
        {
            if(event.is_left_click())
                manager_3D.window_scene->camera.set_orbit_tag(object.tag);
            return {};
        }
        

        switch (event.type)
        {

        case EventType::MouseButton:

            if(event.mouse_button.is_left_down())
            {
                if(object_name.containsPoint(cursor_sane))
                    Print::line("click W::ObjectLarge - not in subwidget");
            }
            break;

        default:
            break;

        }


        return {};
    }

    /** Recreates the whole widget from scene data every call. */
    inline void reload(Object& new_object, f2 new_pos, float width)
    {
        object = new_object;

        // Frame
        frame.pos = new_pos;
        frame.size = {width, H};
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x444444ff);

        float y = frame.pos.y + frame.size.y;
        float x = frame.pos.x + 5.0f;
        float child_width = frame.size.x - 10.0f;
        float y_delta = -10.0f;

        // Name
        y += -20.0f + y_delta;
        object_name.box.pos = {x, y};
        object_name.set_str(object.name);

        // ORBIT TOGGLE
        S_orbit.set_str("Orbit");
        f2 orbit_delta = {frame.size.x-S_orbit.actual_width-5.0f, 0.0f};
        S_orbit.box.pos = f2{x, y} + orbit_delta;

        // POS
        y += -w_pos.H + y_delta;
        w_pos.reload(   new_object.pos, 
                        "Pos", 
                        {x, y},
                        child_width         );

        // ROT
        y += -w_rot.H + y_delta;
        w_rot.reload(   new_object.rot, 
                        "Rot", 
                        {x, y},
                        child_width         );

        // MESH
        y += -w_mesh.H + y_delta;
        w_mesh.reload(  new_object.mesh,
                        {x, y},
                        child_width         );

    }


    inline void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(object_name);
        renderer.draw_base_string(S_orbit);

        w_pos.render(renderer);
        w_rot.render(renderer);

        w_mesh.render(renderer);
    }

};



}
}