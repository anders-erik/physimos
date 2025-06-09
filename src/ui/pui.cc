
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
void PUI::event_all(window::InputEvent& event)
{

    if(is_grabbing_widget()){

        if(is_grabbing(&widget_root_scene))
        {
            EventResult result = widget_root_scene.event_handler(event);
            if(result.is_release())
            {
                grabbed_widget = nullptr;
            }
        }
        if(is_grabbing(&widget_quad))
        {
            EventResult result = widget_quad.event_handler(event);
            if(result.is_release())
            {
                grabbed_widget = nullptr;
            }
        }

        return;
    }

    if(is_hovering_widget()){

        if(is_hovering(&widget_root_scene))
        {
            EventResult result = widget_root_scene.event_handler(event);
            if(result.is_grab())
            {
                grabbed_widget = &widget_root_scene;
            }
        }
        if(is_hovering(&widget_quad))
        {
            EventResult result = widget_quad.event_handler(event);
            if(result.is_grab())
            {
                grabbed_widget = &widget_quad;
            }
        }

    }
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



void PUI::
reload_cursor_target(f2 cursor_pos_win_sane){

    if(is_grabbing_widget())
        return;
    
    hovered_widget = nullptr;


    bool widget_quad_is_hovered = widget_quad.has_cursor(cursor_pos_win_sane);
    if(widget_quad_is_hovered)
    {
        hovered_widget = &widget_quad;
        return;
    }

    // Query widgets first!
    bool widget_is_targeted = widget_root_scene.has_cursor(cursor_pos_win_sane);
    if(widget_is_targeted)
    {
        hovered_widget = &widget_root_scene;
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
event_mouse_move(window::InputEvent& event)
{
    // if(grabbed_base != nullptr)
    // {
        
    //     Box2D box = grabbed_base->get_box();
    //     box.pos += event.mouse_movement.delta.sane;
    //     grabbed_base->set_box(box);

    // }
}

void PUI::
event_mouse_button(window::InputEvent& event)
{
    // if(event.mouse_button.is_left_down())
    // {
        

    //     if(is_targeting_base())
    //     {
    //         targeted_base->click();
    //         grabbed_base = targeted_base;
    //     }
    //     if(is_targeting_widget())
    //     {
    //         hovered_widget->mouse_left_down();
    //         grabbed_widget = hovered_widget;
    //     }
    // }
    // else if(event.mouse_button.is_left_up())
    // {
    //     if(is_grabbing_base())
    //     {
    //         grabbed_base->unclick();
    //         grabbed_base = nullptr;
    //     }
    //     if(is_targeting_widget())
    //     {
    //         hovered_widget->mouse_left_up();
    //         grabbed_widget = nullptr;
    //     }
    // }
}

void PUI::
event_mouse_scroll(window::InputEvent& event)
{
    // if(is_targeting_base())
    // {
    //     targeted_base->scroll(event.mouse_scroll.delta);
    // }
    // else if(is_targeting_widget())
    // {
    //     hovered_widget->scroll(event.mouse_scroll.delta);
    // }
}

void PUI::
event_keystroke(window::InputEvent& event)
{

}

void PUI::
event_window_resize(window::InputEvent& event)
{
    renderer_base.set_window_info(event.window_resize.size_f, event.window_resize.content_scale);
}





void PUI::
reload(){
    widget_root_scene.reload({10.0f, 300.0f});

    // delete widget_quad;
    // widget_quad = new WidgetQuad();
    // widget_quad->reload();
    scene::QuadS2D* quad = ManagerScene::get_root_scene()->quad_manager.get_selected();
    widget_quad.reload(quad, {20.0f, 250.0f});
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

