#pragma once

#include "math/vecmat.hh"

#include "ui/render/renderer_base.hh"

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/widget_root_scene.hh"
#include "ui/widget_quad.hh"

#include "ui/pui_cursor.hh"

namespace UI {


/** Physimos UI. */
class PUI 
{
    UI::RendererBase renderer_base;



    UI::Base base_0;
    UI::Base new_quad_in_root_scene;
    UI::BaseTexture base_texture;
    UI::BaseString base_string;


    UI::WidgetRootScene widget_root_scene;
    UI::WidgetQuad widget_quad;

    PUICursor cursor;

public:

    PUI(f2 window_size_f, f2 content_scale);

    
    /** If cursor is grabbed OR hovering over UI. */
    bool is_targeted_by_cursor();
    /** Ignores cursor location and only indicates whether cursor is grabbed */
    bool is_grabbing_cursor();

    /** 
        Queries the ui for a matching element in which the cursor is contained.
        Internal hover state is set appropriately. 
    */
    bool contains_point(f2 cursor_pos_win_sane);

    void clear_hovers();

    /** Reloads the UI to reflect the most up-to-date data from scenes. */
    void update();

    void event_all(window::InputEvent& event);
    void event_window_resize(window::InputEvent& event);

    void render();

};


}
