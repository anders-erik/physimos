#pragma once

#include "math/vecmat.hh"

#include "ui/render/renderer_base.hh"

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/widget_root_scene.hh"
#include "ui/widget_quad.hh"


namespace UI {


/** Keeps track of hovered and grabbed objects. */
class PUICursor
{


public:

    UI::Widget* grabbed_widget = nullptr;   // widget that has grabbed the cursor
    UI::Widget* hovered_widget = nullptr; // currently targeted widget

    bool is_targeting_widget();
    bool is_hovering_widget();
    bool is_grabbing_widget();
    
    bool is_grabbing(Widget* widget);
    bool is_hovering(Widget* widget);
    /** Hover or grab. */
    bool is_targeted_widget(Widget* widget);
    
    void grab(Widget* widget);
    void hover(Widget* widget);

    void clear_grabbed_widget();
    void clear_hover();
    void clear_all();

    /** Grab, and continue grabbing, only while still returned grab results. Default is release. */
    void handle_event_result(EventResult event_result, Widget* widget);
    
    /** If cursor is grabbed OR hovering over UI. */
    bool is_targeted_by_cursor();
    /** Ignores cursor location and only indicates whether cursor is grabbed */
    bool is_grabbing_cursor();

};


}
