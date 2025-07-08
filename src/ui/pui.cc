
#include "lib/print.hh"
// REND
#include "rend/pui/renderer_base.hh"

#include "scene/scene_state.hh"
#include "scene/manager_3D.hh"

// #include "ui/pui.hh"
#include "pui.hh"

namespace UI {

PUI::PUI() 
{
    // UI::RendererBase& renderer_base = Rend::Manager::get_renderer_pui();
    // renderer_base.set_window_info(window_size_f, content_scale);

    // panel_left.base.box.pos  = {400.0f, 400.0f}; 
    // panel_left.base.box.size = {40.0f, 40.0f}; 


    b_color_texture.set_pos({900, 10});
    f2 color_size = {16.0f, 256.0f};
    b_color_texture.set_size(color_size);

    b_color_sample.set_pos({850, 10});
    b_color_sample.set_size({5, 5});
    b_color_sample.color = 0x009999;
}




bool PUI::
contains_point(f2 cursor_pos_win_sane)
{
    if(panel_left.base.containsPoint(cursor_pos_win_sane))
        return true;
    if(panel_right.base.containsPoint(cursor_pos_win_sane))
        return true;


    // cursor.clear_grabbed_widget();
    // cursor.clear_hover();
    // if(w_object_large.has_cursor(cursor_pos_win_sane))
    // {
    //     cursor.hover(&w_object_large);
    //     return true;
    // }


    return false;
}


void PUI::
clear_hovers()
{
    cursor.clear_hover();
}






void PUI::
reload(Manager3D& manager_3D, f2 framebuffer_size)
{
    panel_left.update(manager_3D, framebuffer_size);
    panel_right.update(manager_3D, framebuffer_size);


    // Object* obj_p = manager_3D.manager_o.get_object(manager_3D.state.selected.tag);
    // if(obj_p != nullptr)
    //     w_object_large.reload(*obj_p, {500.0f, 440.0f});
    // else
    //     w_object_large.frame_base.get_box() = Box2D{};
}




InputResponse PUI::
event_all(Manager3D& manager_3D, window::InputEvent& event, f2 cursor_pos_fb)
{
    if(panel_left.base.containsPoint(cursor_pos_fb))
    {
        return panel_left.event_handler(    manager_3D, 
                                                            event, 
                                                            cursor_pos_fb   );
    }
    if(panel_right.base.containsPoint(cursor_pos_fb))
    {
        return panel_right.event_handler(    manager_3D, 
                                                            event, 
                                                            cursor_pos_fb   );
    }


    // if(cursor.is_targeted_widget(&w_object_large))
    // {
    //     InputResponse result = w_object_large.event_handler(manager_3D, event);
    //     cursor.handle_event_result(result, &w_object_large);
    //     if(result.is_mouse_grab())
    //         return InputResponse(InputResponse::MOUSE_GRAB);
    // }

    return InputResponse();
}




void PUI::
render(UI::RendererBase& renderer_base)
{
    // w_object_large.render(renderer_base);

    panel_left.render(renderer_base);
    panel_right.render(renderer_base);

    renderer_base.draw_base_texture(b_color_texture);
    renderer_base.draw_base_texture(b_color_sample);
}

void PUI::do_action(Actions action)
{
    switch (action)
    {
    case Actions::ToggleLeftPanel :
        panel_left.collapsed = panel_left.collapsed ? false : true;
        break;
    
    case Actions::ToggleRightPanel :
        panel_right.collapsed = panel_right.collapsed ? false : true;
        break;
    }

}




}

