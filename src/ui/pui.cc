
// #include "ui/pui.hh"
#include "pui.hh"

namespace UI {

PUI::PUI(f2 window_size_f, f2 content_scale) 
{
    renderer_base.set_window_info(window_size_f, content_scale);

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
is_targeting_widget()
{
    if(is_grabbing_widget())
        return true;
    if(is_hovering_widget())
        return true;
    return false;
}

bool PUI::
is_targeted_widget(Widget* widget)
{
    if(is_grabbing(widget))
        return true;
    if(is_hovering(widget))
        return true;
    return false;
}

void PUI::grab(Widget * widget)
{
    grabbed_widget = widget;
}

void PUI::hover(Widget * widget)
{
    hovered_widget = widget;
}

bool PUI::
is_hovering_widget()
{
    return hovered_widget == nullptr ? false : true;
}

bool PUI::
is_grabbing_widget()
{
    return grabbed_widget == nullptr ? false : true;
}

bool PUI::
is_grabbing(Widget* widget)
{
    return grabbed_widget == widget ? true : false;
}

bool PUI::is_hovering(Widget * widget)
{
    return hovered_widget == widget ? true : false;
}

void PUI::hover_clear()
{
    hovered_widget = nullptr;
}

void PUI::clear_grabbed_widget()
{
    grabbed_widget = nullptr;
}



void PUI::
reload_cursor_target(f2 cursor_pos_win_sane)
{
    if(is_grabbing_widget())
        return;
    
    hover_clear();

    if(widget_quad.has_cursor(cursor_pos_win_sane))
    {
        hover(&widget_quad);
        return;
    }

    if(widget_root_scene.has_cursor(cursor_pos_win_sane))
    {
        hover(&widget_root_scene);
        return;
    }



    Base* base;

    base = base_0.containsPoint(cursor_pos_win_sane);
    if(base != nullptr){
        // targeted_base->set_hover();
        println("hover base 0");
        return;
    }
    base = new_quad_in_root_scene.containsPoint(cursor_pos_win_sane);
    if(base != nullptr){
        // targeted_base->set_hover();
        println("hover second quad");
        return;
    }


    return;

}

bool PUI::is_targeted_by_cursor()
{
    if(is_targeting_widget())
        return true;

    return false;
}

bool PUI::is_grabbing_cursor()
{
    if(is_grabbing_widget())
        return true;

    return false;
}







void PUI::
reload(){
    widget_root_scene.reload({10.0f, 300.0f});

    // delete widget_quad;
    // widget_quad = new WidgetQuad();
    // widget_quad->reload();
    scene::QuadS2D* quad = ManagerScene::get_root_scene_mut().quad_manager.get_selected();
    widget_quad.reload(quad, {20.0f, 250.0f});
}



void PUI::event_all(window::InputEvent& event)
{
    if(is_targeted_widget(&widget_root_scene))
    {
        EventResult result = widget_root_scene.event_handler(event);
        if(result.is_release())
        {
            clear_grabbed_widget();
        }
        if(result.is_grab())
        {
            grab(&widget_root_scene);
        }
    }

    if(is_targeted_widget(&widget_quad))
    {
        EventResult result = widget_quad.event_handler(event);
        if(result.is_release())
        {
            clear_grabbed_widget();
        }
        if(result.is_grab())
        {
            grab(&widget_root_scene);
        }
    }
}



void PUI::
event_window_resize(window::InputEvent& event)
{
    renderer_base.set_window_info(
        event.window_resize.size_f, 
        event.window_resize.content_scale
    );
}



void PUI::
render(){

    renderer_base.draw_base(base_0);
    renderer_base.draw_base(new_quad_in_root_scene);

    // renderer_base.draw_base_texture(base_texture);
    base_texture.render_texture(renderer_base);
    base_string.render_string(renderer_base);

    // widget_root_scene.populate();
    widget_root_scene.render(renderer_base);

    widget_quad.render(renderer_base);

}




}

