#pragma once

#include "lib/str.hh"

#include "ui/base.hh"
#include "ui/render/renderer_ui.hh"

namespace UI {


struct BaseString : public Base {


    BaseString() {}

    void render_string(UI::RendererBase& renderer){
        // renderer.draw_base((*(Base*)this));
        renderer.draw_base_string(*this);
    }

};


}