
#include "scene/scene3D.hh"
#include "scene2D/scene2D.hh"

#include "rend/scene3D/renderer3D.hh"
#include "rend/scene2D/renderer2D.hh"
#include "rend/pui/renderer_base.hh"

#include "rend/rend_manager.hh"
#include "rend_manager.hh"



alignas(RendererScene3D)        static unsigned char rendererScene3D_buff[sizeof(RendererScene3D)];
alignas(RendererScene2D) static unsigned char rendererScene2D_buff[sizeof(RendererScene2D)];
alignas(UI::RendererBase)       static unsigned char renderer_pui_buff[sizeof(UI::RendererBase)];

RendererScene3D*        rendererScene3D = nullptr;
RendererScene2D* rendererScene2D = nullptr;
UI::RendererBase*       renderer_pui    = nullptr;


void Rend::Manager::
init(f2 window_fb_size)
{
    rendererScene3D = new (rendererScene3D_buff)    RendererScene3D(window_fb_size);
    rendererScene2D = new (rendererScene2D_buff)    RendererScene2D();
    renderer_pui    = new (renderer_pui_buff)       UI::RendererBase();

    // TODO: get content scale
    renderer_pui->set_window_info(window_fb_size, {1, 1});
}


RendererScene3D& Rend::Manager::get_renderer_scene3D()
{
    return *rendererScene3D;
}

RendererScene2D& Rend::Manager::get_renderer_scene2D()
{
    return *rendererScene2D;
}

UI::RendererBase& Rend::Manager::get_renderer_pui()
{
    return *renderer_pui;
}
