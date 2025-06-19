#pragma once

#include "lib/str.hh"

#include "ui/base.hh"
#include "rend/pui/renderer_base.hh"

namespace UI {


struct BaseTexture : public Base {


    BaseTexture() {}

    void render_texture(UI::RendererBase& renderer){
        renderer.draw_base_texture(*this);
    }

};


}