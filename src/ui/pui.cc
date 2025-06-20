
// REND
#include "rend/pui/renderer_base.hh"
#include "rend/rend_manager.hh"


// #include "ui/pui.hh"
#include "pui.hh"

namespace UI {

PUI::PUI() 
{
    // UI::RendererBase& renderer_base = Rend::Manager::get_renderer_pui();
    // renderer_base.set_window_info(window_size_f, content_scale);

    base_0.set_pos({100, 100});
    base_0.set_size({100, 100});

    new_quad_in_root_scene.set_pos({50, 50});
    new_quad_in_root_scene.set_size({50, 50});

    base_texture.set_pos({110, 50});
    base_texture.set_size({50, 50});

    base_string.set_str("Hola");
    base_string.set_pos({170, 50});
}




bool PUI::
contains_point(f2 cursor_pos_win_sane)
{
    cursor.clear_grabbed_widget();
    cursor.clear_hover();

    if(w_object_large.has_cursor(cursor_pos_win_sane))
    {
        cursor.hover(&w_object_large);
        return true;
    }

    if(w_root_scene.has_cursor(cursor_pos_win_sane))
    {
        cursor.hover(&w_root_scene);
        return true;
    }



    Base* base;

    base = base_0.containsPoint(cursor_pos_win_sane);
    if(base != nullptr){
        // targeted_base->set_hover();
        print("0");
        return true;
    }
    base = new_quad_in_root_scene.containsPoint(cursor_pos_win_sane);
    if(base != nullptr){
        // targeted_base->set_hover();
        print("1");
        return true;
    }


    return false;
}

void PUI::
clear_hovers()
{
    cursor.clear_hover();
}






void PUI::
update()
{
    w_root_scene.reload({10.0f, 300.0f});

    // delete widget_quad;
    // widget_quad = new WidgetQuad();
    // widget_quad->reload();
    // scene::QuadS2D* quad = ManagerScene::get_quad_manager().get_selected();
    // w_object_large.reload(quad, {500.0f, 450.0f});

    Scene3D& scene = ManagerScene::get_window_scene_mut();
    w_object_large.reload(scene.selected_object, {500.0f, 450.0f});

    // 
}




InputResponse PUI::
event_all(window::InputEvent& event)
{
    if(cursor.is_targeted_widget(&w_root_scene))
    {
        InputResponse result = w_root_scene.event_handler(event);
        cursor.handle_event_result(result, &w_root_scene);
        if(result.grabbed_mouse())
            return InputResponse(InputResponse::MOUSE_GRAB);
    }

    if(cursor.is_targeted_widget(&w_object_large))
    {
        InputResponse result = w_object_large.event_handler(event);
        cursor.handle_event_result(result, &w_object_large);
        if(result.grabbed_mouse())
            return InputResponse(InputResponse::MOUSE_GRAB);
    }

    return InputResponse();
}




void PUI::
event_window_resize(window::WindowResizeEvent& window_resize)
{
    UI::RendererBase& renderer_base = Rend::Manager::get_renderer_pui();
    renderer_base.set_window_info(
        window_resize.size_f, 
        window_resize.content_scale
    );
}



void PUI::
render(){

    UI::RendererBase& renderer_base = Rend::Manager::get_renderer_pui();

    renderer_base.draw_base(base_0);
    renderer_base.draw_base(new_quad_in_root_scene);

    // renderer_base.draw_base_texture(base_texture);
    base_texture.render_texture(renderer_base);
    base_string.render_string(renderer_base);

    // widget_root_scene.populate();
    w_root_scene.render(renderer_base);

    w_object_large.render(renderer_base);

}




}

