#pragma once

#include "lib/print.hh"

#include "rend/pui/renderer_base.hh"
#include "scene/manager_3D.hh"

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/widget.hh"
#include "ui/widgets/w_camera.hh"
#include "ui/widgets/w_list.hh"
#include "ui/widgets/w_root_scene.hh"

namespace UI {


struct PanelLeft
{
    Base base;

    BaseTexture T_collapse;
    bool collapsed = false;

    UI::W::RootScene w_root_scene;

    float max_width = 300.0f;

    PanelLeft()
    {
    }

    InputResponse 
    event_handler(Manager3D& manager_3D, window::InputEvent& event, f2 cursor_sane)
    {
        if(w_root_scene.has_cursor(cursor_sane))
            return w_root_scene.event_handler(manager_3D, event);

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
        if(collapsed)
        {
            base.box.pos  = {5.0f, framebuffer_size.y - 30.0f};
            base.box.size = {framebuffer_size.x * 0.3f, 30.0f};
            return;
        }

        base.box.pos  = {5.0f, framebuffer_size.y * 0.1f};
        base.box.size = {framebuffer_size.x * 0.3f, framebuffer_size.y * 0.9f};

        if(base.box.size.x > max_width)
            base.box.size.x = max_width;

        float child_width = base.box.size.x - 10.0f;

        float y = base.box.pos.y + base.box.size.y;
        float x = base.box.pos.x + 5.0f;

        float collapse_y = -25.0f;
        T_collapse.box.pos = {x, y + collapse_y};
        T_collapse.box.size = {20.0f, 20.0f};

        float scene_y = -500.0f;
        w_root_scene.reload(manager_3D, {x, y + scene_y}, child_width);
    }



    void 
    render(RendererBase& renderer)
    {
        renderer.draw_base(base);
        renderer.draw_base_texture(T_collapse);

        if(collapsed)
            return;


        w_root_scene.render(renderer);
    }
};


}