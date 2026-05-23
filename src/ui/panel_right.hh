#pragma once

#include "lib/print.hh"

#include "rend/pui/renderer_base.hh"
#include "scene/manager_3D.hh"

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/widget.hh"
#include "ui/widgets/w_object_l.hh"

namespace UI {


struct PanelRight
{
    Base base;

    BaseTexture T_collapse;
    bool collapsed = false;
    bool has_selected = true;

    UI::W::ObjectLarge w_object_l;

    float max_width = 300.0f;


    PanelRight() = default;


    InputResponse 
    event_handler(Manager3D& manager_3D, window::InputEvent& event, f2 cursor_sane)
    {
        if(w_object_l.has_cursor(cursor_sane))
            return w_object_l.event_handler(manager_3D, event);

        if(T_collapse.containsPoint(cursor_sane))
        {
            if(event.is_left_click())
            {
                Print::line("Collapse toggle");
                collapsed = collapsed ? false : true;
            }
        }

        return {};
    }

    void
    update(Manager3D& manager_3D, f2 framebuffer_size)
    {
        Object* obj_p = manager_3D.manager_o.get_object(manager_3D.state.selected.tag);
        has_selected = obj_p != nullptr ? true : false;

        if(collapsed)
        {
            base.box.size = {   framebuffer_size.x * 0.25f, 
                                30.0f                                       };
            base.box.pos  = {   framebuffer_size.x - base.box.size.x - 5.0f, 
                                framebuffer_size.y - 30.0f                  };
            return;
        }

        base.box.size = {   framebuffer_size.x * 0.25f, 
                            framebuffer_size.y * 0.9f                       };
        if(base.box.size.x > max_width)
            base.box.size.x = max_width;

        base.box.pos  = {   framebuffer_size.x - base.box.size.x - 5.0f, 
                            framebuffer_size.y * 0.1f - 5.0f                };


        float child_width = base.box.size.x - 10.0f;

        float y = base.box.pos.y + base.box.size.y;
        float x = base.box.pos.x + 5.0f;

        float collapse_y = -25.0f;
        T_collapse.box.pos = {x, y + collapse_y};
        T_collapse.box.size = {20.0f, 20.0f};

        float object_y = - (w_object_l.H + 40.0f);
        if(has_selected)
            w_object_l.reload(*obj_p, {x, y + object_y}, child_width);
    }



    void 
    render(RendererBase& renderer)
    {
        renderer.draw_base(base);
        renderer.draw_base_texture(T_collapse);

        if(collapsed)
            return;

        if(has_selected)
            w_object_l.render(renderer);
    }

};


}