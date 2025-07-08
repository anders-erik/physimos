
#pragma once

#include "lib/pair.hh"

#include "math/quarternion.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"


#include "widget.hh"


namespace UI 
{
namespace W 
{



/** Widget reflecting the state of a specific Quarternion */
struct Q : public Widget
{
    Quarternion dummy_q = {1.0f, 0.0f, 0.0f, 0.0f}; // dummy f3
    static constexpr float H = 25.0f;

    Str str_label = "Rot: ";

    float scroll_step = 1.0f; // delta induced by single scroll event
    uchar decimal_points = 2; // Number of digits past the point

    BaseString base_label;
    BaseString base_q0;
    BaseString base_x;
    BaseString base_y;
    BaseString base_z;

public:


    inline InputResponse event_handler(window::InputEvent& event)
    {
        using namespace window;

        switch (event.type)
        {
        
        case EventType::MouseScroll:

            // if(base_q0.containsPoint(cursor_sane))
            // {
            //     if(event.mouse_scroll.delta > 0.0f)
            //         dummy_q.x += scroll_step;
            //     else
            //         dummy_q.x -= scroll_step;
            // }
            // if(base_x.containsPoint(cursor_sane))
            // {
            //     if(event.mouse_scroll.delta > 0.0f)
            //         dummy_q.x += scroll_step;
            //     else
            //         dummy_q.x -= scroll_step;
            // }
            // else if(base_y.containsPoint(cursor_sane))
            // {
            //     if(event.mouse_scroll.delta > 0.0f)
            //         dummy_q.y += scroll_step;
            //     else
            //         dummy_q.y -= scroll_step;
            // }
            // else if(base_z.containsPoint(cursor_sane))
            // {
            //     if(event.mouse_scroll.delta > 0.0f)
            //         dummy_q.z += scroll_step;
            //     else
            //         dummy_q.z -= scroll_step;
            // }
            break;

        default:
            break;

        }

        return {};
    }

    /** Recreates the whole widget from scene data every call. */
    inline void reload(Quarternion& q, Str label, f2 new_pos, float W)
    {
        dummy_q = {1.0f, 0.0f, 0.0f, 0.0f};

        // Frame
        frame.pos = new_pos;
        frame.size = {W, H};
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x836343ff);


        f2 base_offset = frame.pos;

        // POS
        f2 pos_label_delta = { 5.0f, 5.0f };
        f2 pos_q0_delta = { frame.size.x * 1.0f / 5.0f, 5.0f };
        f2 pos_x_delta  = { frame.size.x * 2.0f / 5.0f, 5.0f };
        f2 pos_y_delta  = { frame.size.x * 3.0f / 5.0f, 5.0f };
        f2 pos_z_delta  = { frame.size.x * 4.0f / 5.0f, 5.0f };

        // label
        base_label.set_pos(base_offset + pos_label_delta);
        base_label.set_str(label);
        // q0
        base_q0.set_pos(base_offset + pos_q0_delta);
        base_q0.set_str(Str::FL(q.q0, decimal_points, Str::FloatRep::Fixed));
        // x
        base_x.set_pos(base_offset + pos_x_delta);
        base_x.set_str(Str::FL(q.x, decimal_points, Str::FloatRep::Fixed));
        // y
        base_y.set_pos(base_offset + pos_y_delta);
        base_y.set_str(Str::FL(q.y, decimal_points, Str::FloatRep::Fixed));
        // z
        base_z.set_pos(base_offset + pos_z_delta);
        base_z.set_str(Str::FL(q.z, decimal_points, Str::FloatRep::Fixed));
    }


    inline void render(RendererBase& renderer)
    {
        renderer.draw_base(frame_base);

        renderer.draw_base_string(base_label);
        renderer.draw_base_string(base_q0);
        renderer.draw_base_string(base_x);
        renderer.draw_base_string(base_y);
        renderer.draw_base_string(base_z);
    }

};



}
}