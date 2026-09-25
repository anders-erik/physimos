
#pragma once

#include "lib/arr.hh"

#include "io/input/user_input.hh"
#include "ui/ui4/ui.hh"

#include "rend/sw/swrend.hh"


struct UI;
struct PianoApp;

/** 
    The ui of the piano app.
    Controls the UI backend and renders using SWR::renderer for rendering.
 */
class PianoUI
{

public:

    UI ui;
    SWR::Renderer renderer;

    PianoUI()
    {
    }

    PianoUI(PianoApp* _piano_app, PixelBuffer _pixel_buffer)
    {
        init(_piano_app, _pixel_buffer);
    } 

    void init(PianoApp* _piano_app, PixelBuffer _pixel_buffer);

    void render_ui_node(UINode* _node);
    void render();

    bool contains_pointer_pos(d2 _pointer_pos)
    {
        return (ui.get_element_at_pos(_pointer_pos) == nullptr) ? false : true;
    }
};
