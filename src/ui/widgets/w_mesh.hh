
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



namespace UI 
{
namespace W 
{



/** Widget reflecting the state of a specific quad */
struct Mesh : public Widget
{
    ::Mesh mesh; // object copy - read only
    static constexpr float H = 70.0f;

    BaseString name;
    
    BaseString verts_size_label;
    BaseString verts_size;

    BaseString faces_size_label;
    BaseString faces_size;


public:


    /** Recreates the whole widget from scene data every call. */
    inline void reload(::Mesh& new_mesh, f2 new_pos, float width)
    {
        mesh = new_mesh;

        // Frame
        frame.pos = new_pos;
        frame.size = {width, H};
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x222222ff);

        // float y = frame.pos.y + frame.size.y;
        // float x = frame.pos.x + 5.0f;
        // float child_width = frame.size.x - 10.0f;

        f2 offset = frame.pos;
        offset += {5.0f, frame.size.y};

        // Name
        f2 name_delta = { 0.0f, -20.0f };
        name.set_pos(offset += name_delta);
        name.set_str("Mesh");

        
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
        verts_size.set_str(Str::SI(mesh.verts.size()));


        // label
        f2 faces_label_delta = { 10.0f, -40.0f };
        faces_size_label.set_pos(offset + faces_label_delta);
        faces_size_label.set_str(Str("Faces: "));
        // y
        f2 faces_size_delta = { 150.0f, -40.0f };
        faces_size.set_pos(offset + faces_size_delta);
        faces_size.set_str(Str::SI(mesh.faces.size()));

        offset -= box_indent;

    }


    inline void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(name);

        renderer.draw_base_string(verts_size_label);
        renderer.draw_base_string(verts_size);

        renderer.draw_base_string(faces_size_label);
        renderer.draw_base_string(faces_size);
    }

};



}
}