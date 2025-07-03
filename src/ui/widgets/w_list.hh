
#pragma once

#include <vector>

#include "lib/pair.hh"

#include "rend/pui/renderer_base.hh"

#include "ui/string.hh"


#include "widget.hh"


namespace UI 
{
namespace W 
{



template <typename Wi, typename Ob, float height>
struct List : public Widget
{
    std::vector<Wi> widgets;

    f2 size = {200.0f, height};

    Str str_label = "List Widget";

    float scroll        = 0.0f; // total scroll offset from zero
    float scroll_step   = 10.0f; // delta induced by single scroll event
    float scroll_max    = 0.0f;
    float scroll_min    = 0.0f;

public:


    inline InputResponse event_handler(Manager3D& manager_3D, window::InputEvent& event)
    {
        using namespace window;

        switch (event.type)
        {

        case EventType::MouseScroll:
            if(event.mouse_scroll.delta > 0.0f)
                scroll += scroll_step;
            else
                scroll -= scroll_step;

            // clamp
            if(scroll > scroll_max)
                scroll = scroll_max;
            if(scroll < scroll_min)
                scroll = scroll_min;
            break;

        default:
            for(auto& w : widgets)
            {
                if(w.has_cursor(cursor_sane))
                    return w.event_handler(manager_3D, event);
            }
            break;

        }

        return {};
    }

    /** Recreates the whole widget from scene data every call. */
    inline void reload(std::vector<Ob>& objects, f2 new_pos)
    {
        // Frame
        frame.pos  = new_pos;
        frame.size = size;
        frame_base.set_box(frame);
        frame_base.set_rgba_color(0x723556ff);

        widgets.clear();

        f2 offset = {   frame.pos.x, 
                        frame.pos.y + size.y + scroll };
        float y_spacing = 1.0f;

        for(auto& ob : objects)
        {
            Wi& wi = widgets.emplace_back();
            f2 wi_offset = {0.0f, wi.size.y + y_spacing};
            wi.reload(ob, offset -= wi_offset);
        }
        
        float widgets_render_height = widgets.size() * (Wi::size.y + y_spacing);
        float available_scroll = frame.size.y - widgets_render_height;
        scroll_max = available_scroll > 0.0f ? 0.0f : -available_scroll;
    }


    inline void render(RendererBase& renderer)
    {
        renderer.stencil_start_new_capture();

        renderer.draw_base(frame_base);

        renderer.stencil_apply();

        for(auto& w : widgets)
            w.render(renderer);

        renderer.stencil_disable();
    }

};



}
}