#pragma once

#include "math/vecmat.hh"

struct RendererScene3D;
namespace scene {
    struct RendererScene2D;
}
namespace UI {
    struct RendererBase;
}



namespace Rend::Manager
{


enum Types
{
    Scene3D,
    Scene2D,
};


void init(f2 window_fb_size);

RendererScene3D&            get_renderer_scene3D();
scene::RendererScene2D&     get_renderer_scene2D();
UI::RendererBase&           get_renderer_pui();


}

