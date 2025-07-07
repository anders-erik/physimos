#pragma once

#include "math/vecmat.hh"

#include "window/auxevent.hh"

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/widgets/w_root_scene.hh"
#include "ui/widgets/w_camera.hh"
#include "ui/widgets/w_object_s.hh"
#include "ui/widgets/w_object_l.hh"
#include "ui/widgets/w_list.hh"

#include "ui/pui_cursor.hh"

struct Scene3D;
struct Manager3D;
struct SceneState;

struct RendererBase;

namespace UI {


/** Physimos UI. */
class PUI 
{
    UI::Base base_0;
    UI::Base new_quad_in_root_scene;
    UI::BaseTexture b_color_texture;
    UI::BaseTexture b_color_sample;
    UI::BaseString base_string;


    UI::W::RootScene w_root_scene;
    UI::W::ObjectLarge w_object_large;

    UI::W::List<UI::W::ObjectSmall, Object, 150.0f> list_object;
    UI::W::Camera w_camera;

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
    void reload(Manager3D& manager_3D, f2 framebuffer_size);

    InputResponse event_all(Manager3D& manager_3D, window::InputEvent& event);

    void render(UI::RendererBase& renderer_base);

};


}
