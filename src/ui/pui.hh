#pragma once

#include "math/vecmat.hh"

#include "window/auxevent.hh"

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/widgets/w_root_scene.hh"
#include "ui/widgets/w_object_s.hh"
#include "ui/widgets/w_object_l.hh"

#include "ui/pui_cursor.hh"


namespace UI {


/** Physimos UI. */
class PUI 
{
    UI::Base base_0;
    UI::Base new_quad_in_root_scene;
    UI::BaseTexture base_texture;
    UI::BaseString base_string;


    UI::W::RootScene w_root_scene;
    UI::W::ObjectLarge w_object_large;

    PUICursor cursor;

public:

    PUI();

    /** 
        Queries the ui for a matching element in which the cursor is contained.
        Cursor position is also copied for later event handling.
    */
    bool contains_point(f2 cursor_pos_win_sane);

    void clear_hovers();

    /** Reloads the UI to reflect the most up-to-date data from scenes. */
    void update();

    InputResponse event_all(window::InputEvent& event);
    void event_window_resize(window::WindowResizeEvent& window_resize);

    void render();

};


}
