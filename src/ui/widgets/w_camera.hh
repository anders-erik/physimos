
#pragma once

#include "lib/pair.hh"
#include "lib/print.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"

#include "scene/object.hh"

#include "scene/manager_3D.hh"
#include "scene/manager_object.hh"

#include "widget.hh"

#include "widgets/w_f2.hh"
#include "widgets/w_f3.hh"


namespace UI 
{
namespace W 
{



/** Widget reflecting the state of a specific quad */
struct Camera : public Widget
{
    CameraObject camobj; // object copy - read only

    static constexpr f2 size = {200.0f, 100.0f};

    BaseString object_name;
    
    BaseString free_label;
    BaseString free_toggle;

    BaseString orbit_center_label;
    BaseString orbit_center_toggle;

    BaseString B_orbit_tag_label;
    BaseString B_orbit_tag_toggle;


public:


    InputResponse 
    event_handler(Manager3D& manager_3D, window::InputEvent& event)
    {
        if(!event.is_left_click())
            return {};


        if(free_toggle.containsPoint(cursor_sane))
        {
            manager_3D.window_scene->camobj.set_free();
        }
        else if(orbit_center_toggle.containsPoint(cursor_sane))
        {
            if(manager_3D.window_scene->camobj.state.is_orbit_center())
                manager_3D.window_scene->camobj.set_free();
            else
                manager_3D.window_scene->camobj.set_orbit_center();
        }
        else if(B_orbit_tag_toggle.containsPoint(cursor_sane))
        {
            if(manager_3D.window_scene->camobj.state.is_orbit_tag())
                manager_3D.window_scene->camobj.set_free();
        }

        return {};
    }

    /** Recreates the whole widget from scene data every call. */
    void 
    reload(CameraObject& new_camobj, f2 new_pos)
    {
        camobj = new_camobj;

        // Frame
        frame.pos = new_pos;
        frame.size = size;
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x444444ff);


        f2 offset = frame.pos;
        offset += {5.0f, frame.size.y};

        // Name
        f2 name_delta = { 0.0f, -20.0f };
        Str name_str = camobj.object.name;
        object_name.set_pos(offset += name_delta);
        object_name.set_str(name_str);

        
        f2 box_indent = { 10.0f, 0.0f };
        offset += box_indent;


        // FREE

        // label
        f2 free_label_delta = { 10.0f, -20.0f };
        free_label.set_pos(offset + free_label_delta);
        free_label.set_str(Str("Free: "));
        // x
        f2 free_delta = { 150.0f, -20.0f };
        free_toggle.set_pos(offset + free_delta);
        if(camobj.state.is_free())
            free_toggle.set_str(" X ");
        else
            free_toggle.set_str(" O ");


        // ORB CENTER
        f2 orbc_label_delta = { 10.0f, -40.0f };
        orbit_center_label.set_pos(offset + orbc_label_delta);
        orbit_center_label.set_str(Str("Orbit Center: "));

        f2 orbc_delta = { 150.0f, -40.0f };
        orbit_center_toggle.set_pos(offset + orbc_delta);
        if(camobj.state.is_orbit_center())
            orbit_center_toggle.set_str(" X ");
        else
            orbit_center_toggle.set_str(" O ");

        // ORB TAG
    
        f2 orbt_label_delta = { 10.0f, -60.0f };
        B_orbit_tag_label.set_pos(offset + orbt_label_delta);
        B_orbit_tag_label.set_str(Str("Orbit Tag: "));
        // y
        f2 orbt_delta = { 150.0f, -60.0f };
        B_orbit_tag_toggle.set_pos(offset + orbt_delta);
        if(camobj.state.is_orbit_tag())
            B_orbit_tag_toggle.set_str(" X ");
        else
            B_orbit_tag_toggle.set_str(" O ");


        offset -= box_indent;
    }


    void 
    render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(object_name);

        renderer.draw_base_string(free_label);
        renderer.draw_base_string(free_toggle);

        renderer.draw_base_string(orbit_center_label);
        renderer.draw_base_string(orbit_center_toggle);

        renderer.draw_base_string(B_orbit_tag_label);
        renderer.draw_base_string(B_orbit_tag_toggle);
    }

};



}
}