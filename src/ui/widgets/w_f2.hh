
#pragma once

#include "lib/pair.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"


#include "widget.hh"


namespace UI 
{
namespace W 
{



/** Widget reflecting the state of a specific quad */
struct F2 : public Widget
{
    f2 event_delta = {0.0f, 0.0f};
    f2 size = {240.0f, 25.0f};

    Str str_label = "f2_label";

    float scroll_step = 10.0f; // delta induced by single scroll event
    int decimal_points = 2; // Number of digits past the point

    BaseString base_label;
    BaseString base_x;
    BaseString base_y;

public:


    inline InputResponse event_handler(window::InputEvent& event)
    {
        using namespace window;

        switch (event.type)
        {

        case EventType::MouseButton:
            if(event.mouse_button.is_left_down())
            {
                // if(quad_name.containsPoint(cursor_sane))
                // {
                //     auto& q_manager = ManagerScene::get_quad_manager();
                //     q_manager.set_selected(quad_id);
                // }
                

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
        
            if(base_x.containsPoint(cursor_sane))
            {
                if(event.mouse_scroll.delta > 0.0f)
                    event_delta.x += scroll_step;
                else
                    event_delta.x -= scroll_step;
            }
            else if(base_y.containsPoint(cursor_sane))
            {
                if(event.mouse_scroll.delta > 0.0f)
                    event_delta.y += scroll_step;
                else
                    event_delta.y -= scroll_step;
            }
            break;

        default:
            break;

        }

        return {};
    }

    /** Recreates the whole widget from scene data every call. */
    inline void reload(f2& _f2, f2 new_pos)
    {
        // Update with info from new events
        _f2 += event_delta;
        event_delta = {0.0f, 0.0f};

        // Frame
        frame.pos = new_pos;
        frame.size = size;
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x836343ff);


        f2 base_offset = frame.pos;

        // POS
        f2 pos_label_delta = { 5.0f, 5.0f };
        f2 pos_x_delta = { size.x / 3.0f, 5.0f };
        f2 pos_y_delta = { size.x * 2.0f / 3.0f, 5.0f };

        // label
        base_label.set_pos(base_offset + pos_label_delta);
        base_label.set_str(str_label);
        // x
        base_x.set_pos(base_offset + pos_x_delta);
        base_x.set_str(Str::FL(_f2.x, decimal_points));
        // y
        base_y.set_pos(base_offset + pos_y_delta);
        base_y.set_str(Str::FL(_f2.y, decimal_points));
    }


    inline void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(base_x);
        renderer.draw_base_string(base_y);
        renderer.draw_base_string(base_label);
    }

};



}
}