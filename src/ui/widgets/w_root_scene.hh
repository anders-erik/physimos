
#pragma once

#include "lib/pair.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/quadS2D.hh"

#include "scene/object_manager.hh"

#include "widget.hh"

#include "widgets/w_object_s.hh"


namespace UI {
namespace W {

/** Lists root scene objects */
struct RootScene : public Widget {

    std::vector<W::ObjectSmall> object_widgets;

    BaseString scene_name_base;

public:

    RootScene() = default;


    InputResponse event_handler(window::InputEvent& event)
    {

        for(auto& quad_widget : object_widgets)
        {
            if(quad_widget.has_cursor(cursor_sane))
            {
                quad_widget.event_handler(event);
            }
        }

        return {};
    }



    /** Recreates the whole widget from scene data every call. */
    void reload(f2 widget_pos)
    {
        Scene3D& root_scene = ManagerScene::get_window_scene_mut();



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


        // Objects
        object_widgets.clear();

        f2 object_w_delta = { 0.0f, -32.0f };
        f2 object_indent = {10.0f, 0.0f};
        
        pos += object_indent;
        for(auto& object : root_scene.get_objects_mut())
        {
            W::ObjectSmall object_widget;
            object_widget.reload(object, pos += object_w_delta);
            object_widgets.push_back(object_widget);
        }
        pos -= object_indent;
    }


    void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(scene_name_base);

        for(auto& object_widget : object_widgets)
            object_widget.render(renderer);
    }
};


}
}