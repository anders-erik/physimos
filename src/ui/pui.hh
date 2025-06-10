#pragma once

#include "math/vecmat.hh"

#include "ui/render/renderer_base.hh"

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/widget_root_scene.hh"
#include "ui/widget_quad.hh"


namespace UI {


/** Physimos UI. */
class PUI 
{
    UI::RendererBase renderer_base;



    UI::Base base_0;
    UI::Base new_quad_in_root_scene;
    UI::BaseTexture base_texture;
    UI::BaseString base_string;

    UI::Widget* grabbed_widget = nullptr;   // widget that has grabbed the cursor
    UI::Widget* hovered_widget = nullptr; // currently targeted widget

    UI::WidgetRootScene widget_root_scene;
    UI::WidgetQuad widget_quad;


    bool is_targeting_widget();
    bool is_hovering_widget();
    bool is_grabbing_widget();
    void hover_clear();
    void clear_grabbed_widget();
    bool is_grabbing(Widget* widget);
    bool is_hovering(Widget* widget);
    bool is_targeted_widget(Widget* widget);
    void grab(Widget* widget);
    void hover(Widget* widget);

public:

    PUI(f2 window_size_f, f2 content_scale);

    
    /** If cursor is grabbed OR hovering over UI. */
    bool is_targeted_by_cursor();
    /** Ignores cursor location and only indicates whether cursor is grabbed */
    bool is_grabbing_cursor();

    /** 
        Queries the ui for a matching element in which the cursor is contained.
        Targets remain valid until another reload() is issued.
        If no grabbet targets available, the hovered target is always reset.
     */
    void reload_cursor_target(f2 cursor_pos_win_sane);

    /** Reloads the UI to reflect the most up-to-date data from scenes. */
    void reload();

    void event_all(window::InputEvent& event);
    void event_window_resize(window::InputEvent& event);

    void render();

};


}
