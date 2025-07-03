
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


namespace UI 
{
namespace W 
{



/** Widget reflecting the state of a specific quad */
struct ObjectLarge : public Widget
{
    Object object; // object copy - read only
    static constexpr f2 size = {250.0f, 150.0f};

    BaseString object_name;
    
    BaseString verts_size_label;
    BaseString verts_size;

    BaseString faces_size_label;
    BaseString faces_size;

    f2 dummy_f2 = {1.0f, 2.0f};
    W::F2 w_f2;
    W::F3 w_pos;

public:


    inline InputResponse event_handler(window::InputEvent& event)
    {
        using namespace window;

        if(w_f2.has_cursor(cursor_sane))
        {
            w_f2.event_handler(event);
        }
        if(w_pos.has_cursor(cursor_sane))
        {
            w_pos.event_handler(event);
        }
        

        switch (event.type)
        {

        case EventType::MouseButton:

            // Name label grabs mouse
            if(event.mouse_button.is_left_down())
            {
                if(object_name.containsPoint(cursor_sane))
                {
                    // auto& q_manager = ManagerScene::get_quad_manager();
                    // q_manager.set_selected(quad_id);
                    is_grabbing_mouse = true;
                    return InputResponse::MOUSE_GRAB;
                }
            }
            else if(event.mouse_button.is_left_up())
            {   
                is_grabbing_mouse = false;
            }
            break;

        
        case EventType::MouseScroll:

            if(verts_size.containsPoint(cursor_sane))
            {
                // auto& q_manager = ManagerScene::get_quad_manager();
                // scene::QuadS2D* quad = q_manager.get_quad_mut(quad_id);
                // if(quad != nullptr)
                // {
                //     Box2D updated_box = quad->get_box();

                //     if(event.mouse_scroll.delta > 0.0f)
                //         updated_box.pos.x += 10.0f;
                //     else
                //         updated_box.pos.x -= 10.0f;
                    
                //     quad->set_box(updated_box.pos, updated_box.size);
                // }

                println("Scroll W::ObjectLarge");
            }
            break;

        default:
            break;

        }


        if(is_grabbing_mouse)
        {   
            return InputResponse::MOUSE_GRAB;
        }

        return {};
    }

    /** Recreates the whole widget from scene data every call. */
    inline void reload(Object& new_object, f2 new_pos)
    {
        object = new_object;

        // Frame
        frame.pos = new_pos;
        frame.size = size;
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x444444ff);


        f2 offset = frame.pos;
        offset += {5.0f, frame.size.y};

        // Name
        f2 name_delta = { 0.0f, -20.0f };
        Str name_str = object.name;
        object_name.set_pos(offset += name_delta);
        object_name.set_str(name_str);

        
        f2 box_indent = { 10.0f, 0.0f };
        offset += box_indent;


        // POS

        // label
        f2 verts_label_delta = { 10.0f, -20.0f };
        verts_size_label.set_pos(offset + verts_label_delta);
        verts_size_label.set_str(Str("Verts: "));
        // x
        f2 pos_x_delta = { 150.0f, -20.0f };
        verts_size.set_pos(offset + pos_x_delta);
        verts_size.set_str(Str::SI(object.mesh.verts.size()));


        // label
        f2 faces_label_delta = { 10.0f, -40.0f };
        faces_size_label.set_pos(offset + faces_label_delta);
        faces_size_label.set_str(Str("Faces: "));
        // y
        f2 faces_size_delta = { 150.0f, -40.0f };
        faces_size.set_pos(offset + faces_size_delta);
        faces_size.set_str(Str::SI(object.mesh.faces.size()));

        offset -= box_indent;

        // F2
        f2 f2_offset = { 0.0f, -80.0f };
        w_f2.reload(dummy_f2, offset + f2_offset);

        // POS
        f2 pos_offset = { 0.0f, -105.0f };
        w_pos.reload(new_object.pos, "Pos", offset + pos_offset);
    }


    inline void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(object_name);

        renderer.draw_base_string(verts_size_label);
        renderer.draw_base_string(verts_size);

        renderer.draw_base_string(faces_size_label);
        renderer.draw_base_string(faces_size);

        w_f2.render(renderer);
        w_pos.render(renderer);
    }

};



}
}