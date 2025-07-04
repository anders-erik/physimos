#pragma once

#include "lib/str.hh"

#include "ui/base.hh"
#include "rend/pui/renderer_base.hh"

using TextureColor = int;

namespace UI {


struct BaseTexture : public Base 
{
    TextureColor color = -1;

    BaseTexture() {}

    void render_texture(UI::RendererBase& renderer){
        renderer.draw_base_texture(*this);
    }

};


}