#pragma once


#include "Windowing.hpp"
#include "Input.hpp"

#include "math/vecmat.hh"

#include "ui/render/renderer_base.hh"

#include "conductor_common.hh"
#include "conductor_viewport.hh"

#include "ui/ui_globals.hh"

#include "ui/ui_primitive.hh"

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/widget_root_scene.hh"

namespace UI {


/** Physimos UI. */
class PUI {

    UI::RendererBase renderer_base;

    /** Grab target across frames. */
    UI::Base* grabbed_base = nullptr;
    /** The Base we are targeting after trying to find target. */
    UI::Base* targeted_base = nullptr;


    UI::Base base_0;
    UI::Base new_quad_in_root_scene;
    UI::BaseTexture base_texture;
    UI::BaseString base_string;

    UI::WidgetRootScene widget_root_scene;


public:

    PUI();

    void set_window_info(f2 size, f2 scale);
    /** has_grabbed_target || has_non_grabbed_target */
    bool has_target();
    bool has_grabbed_target();
    bool has_hover_target();


    void event_mouse_down();
    void event_mouse_up();
    void event_move(f2 cursor_delta);
    void event_scroll(float delta);

    /** Update the active widgets to contain the current scene data. */
    void update();
    /** Queries the ui for a Base matching the cursor pos in window coords.
        Does nothing in grabbed state. 
        Targets remain valid until another update() is issued. 
     */
    void reload_current_targets(f2 cursor_pos_win_sane);

    void render();

};


}
