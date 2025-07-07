
#pragma once

#include "lib/pair.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"

#include "scene2D/scene2D.hh"
#include "scene2D/quadS2D.hh"

#include "scene/manager_3D.hh"
#include "scene/manager_object.hh"

#include "widget.hh"

#include "ui/widgets/w_list.hh"
#include "widgets/w_object_s.hh"
#include "ui/widgets/w_camera.hh"


namespace UI {
namespace W {

/** Lists root scene objects */
struct RootScene : public Widget 
{
    UI::W::List<UI::W::ObjectSmall, Object, 200.0f> list_object;
    UI::W::Camera w_camera;

    BaseString scene_name_base;

public:

    RootScene() = default;


    InputResponse event_handler(Manager3D& manager_3D, window::InputEvent& event)
    {        
        if(w_camera.has_cursor(cursor_sane))
            return w_camera.event_handler(manager_3D, event);

        if(list_object.has_cursor(cursor_sane))
            return list_object.event_handler(manager_3D, event);

        return {};
    }



    /** Recreates the whole widget from scene data every call. */
    void reload(Manager3D& manager_3D, f2 widget_pos, float width)
    {
        // Frame
        frame.pos = widget_pos;
        frame.size = {width, 450.0f};
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x333333ff);


        float y = frame.pos.y + frame.size.y;
        float x = frame.pos.x + 5.0f;

        float name_y = -20.0f;
        scene_name_base.set_pos( {x, y + name_y} );
        scene_name_base.set_str(manager_3D.root_scene.name);

        float objs_y = -250.0f;
        list_object.reload(manager_3D.manager_o.objects, {x, y + objs_y});

        float cam_y = -400.0f;
        w_camera.reload(manager_3D.window_scene->camobj, {x, y + cam_y});        
    }


    void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(scene_name_base);

        list_object.render(renderer);
        w_camera.render(renderer);
    }
};


}
}