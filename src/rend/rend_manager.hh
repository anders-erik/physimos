#pragma once

#include "math/vecmat.hh"

struct RendererScene2D;

namespace UI {
    struct RendererBase;
}



namespace Rend::Manager
{


void init(f2 window_fb_size);

RendererScene2D&    get_renderer_scene2D();
UI::RendererBase&   get_renderer_pui();



}

