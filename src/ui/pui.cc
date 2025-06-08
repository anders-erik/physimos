
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


};


bool PUI::
is_targeting_base(){

    if(is_grabbing_base())
        return true;
    if(is_hovering_base())
        return true;
    return false;

}

bool PUI::
is_grabbing_base()
{
    return grabbed_base == nullptr ? false : true;
}

bool PUI::
is_hovering_base()
{
    return targeted_base == nullptr ? false : true;
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
    return widget_cursor_target == nullptr ? false : true;
}

bool PUI::
is_grabbing_widget()
{
    return widget_cursor_grab == nullptr ? false : true;
}



void PUI::
reload_cursor_target(f2 cursor_pos_win_sane){

    if(is_grabbing_base())
        return;
    
    Base* base;

    // Query widgets first!
    bool widget_is_targeted = widget_root_scene.has_cursor(cursor_pos_win_sane);
    if(widget_is_targeted)
    {
        widget_cursor_target = &widget_root_scene;

        if(is_targeting_base())
        {
            targeted_base->clear_state_flags();
            targeted_base = nullptr;
        }

        return;
    }
    else
    {
        widget_cursor_target = nullptr;
    }

    base = base_0.containsPoint(cursor_pos_win_sane);
    if(base != nullptr){
        targeted_base = &base_0;
        targeted_base->set_hover();
        return;
    }
    base = new_quad_in_root_scene.containsPoint(cursor_pos_win_sane);
    if(base != nullptr){
        targeted_base = &new_quad_in_root_scene;
        targeted_base->set_hover();
        return;
    }


    // clear current target
    if(is_targeting_base()){
        targeted_base->clear_state_flags();
        targeted_base = nullptr;
    }

    return;

}

bool PUI::is_targeted_by_cursor()
{
    if(is_targeting_base())
        return true;
    else if(is_targeting_widget())
        return true;

    return false;
}

bool PUI::is_grabbing_cursor()
{
    if(is_grabbing_base())
        return true;
    else if(is_grabbing_widget())
        return true;

    return false;
}





void PUI::
event_mouse_move(window::InputEvent& event)
{
    if(grabbed_base != nullptr)
    {
        
        Box2D box = grabbed_base->get_box();
        box.pos += event.mouse_movement.delta.sane;
        grabbed_base->set_box(box);

    }
}

void PUI::
event_mouse_button(window::InputEvent& event)
{
    if(event.mouse_button.is_left_down())
    {
        if(is_targeting_base())
        {
            targeted_base->click();
            grabbed_base = targeted_base;
        }
        if(is_targeting_widget())
        {
            widget_cursor_target->grab();
            widget_cursor_grab = widget_cursor_target;
        }
    }
    else if(event.mouse_button.is_left_up())
    {
        if(is_grabbing_base())
        {
            grabbed_base->unclick();
            grabbed_base = nullptr;
        }
        if(is_targeting_widget())
        {
            widget_cursor_target->release();
            widget_cursor_grab = nullptr;
        }
    }
}

void PUI::
event_mouse_scroll(window::InputEvent& event)
{
    if(is_targeting_base())
    {
        targeted_base->scroll(event.mouse_scroll.delta);
    }
    else if(is_targeting_widget())
    {
        widget_cursor_target->scroll(event.mouse_scroll.delta);
    }
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
    widget_root_scene.reload();
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

}




}

