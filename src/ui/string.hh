#pragma once

#include "lib/str.hh"

#include "ui/base.hh"
#include "ui/render/renderer_base.hh"

namespace UI {


struct BaseString : public Base {

    Str str = "DEFAULT STRING NAME";

    BaseString() {}

    void set_str(const Str& new_str){
        str = new_str;
    }

    /** Temporary convenient render method. BaseString owner should instead pass object directly to renderer. */
    void render_string(UI::RendererBase& renderer){
        // renderer.draw_base((*(Base*)this));
        renderer.draw_base_string(*this);
    }

};


}