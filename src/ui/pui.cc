
// #include "ui/pui.hh"
#include "pui.hh"

namespace UI {

PUI::PUI() 
{
    // renderer_base = UI::RendererPrimitive();
    // renderer_base.set_window_info({},{});


    base_0.set_pos({100, 100});
    base_0.set_size({100, 100});

    new_quad_in_root_scene.set_pos({50, 50});
    new_quad_in_root_scene.set_size({50, 50});

    base_texture.set_pos({110, 50});
    base_texture.set_size({50, 50});

    base_string.set_str("Hola");
    base_string.set_pos({170, 50});


};


void PUI::set_window_info(f2 size, f2 scale){
    renderer_base.set_window_info(size, scale);
}

bool PUI::has_target(){

    if(has_grabbed_target())
        return true;
    if(has_hover_target())
        return true;
    return false;

}

bool PUI::has_grabbed_target(){

    return grabbed_base == nullptr ? false : true;

}

bool PUI::has_hover_target(){

    return targeted_base == nullptr ? false : true;

}


void PUI::update_current_targets(f2 cursor_pos_win_sane){

    if(has_grabbed_target())
        return;
    
    Base* base;

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
    if(targeted_base != nullptr){
        targeted_base->clear_state_flags();
        targeted_base = nullptr;
    }

    return;

}




void PUI::event_mouse_down()
{

    if(has_target())
    {
        targeted_base->mouse_down();
        grabbed_base = targeted_base;
    }

}
void PUI::event_mouse_up()
{

    if(grabbed_base != nullptr)
    {
        grabbed_base->mouse_up();
        grabbed_base = nullptr;
    }

}

void PUI::event_move(f2 cursor_delta)
{

    if(grabbed_base != nullptr)
    {
        
        Box box = grabbed_base->get_box();
        box.pos += cursor_delta;
        grabbed_base->set_box(box);

    }

}

void PUI::event_scroll(float delta)
{

    if(targeted_base != nullptr)
    {
        targeted_base->scroll(delta);
    }

}


void PUI::render(){

    renderer_base.draw_base(base_0);
    renderer_base.draw_base(new_quad_in_root_scene);

    // renderer_base.draw_base_texture(base_texture);
    base_texture.render_texture(renderer_base);
    base_string.render_string(renderer_base);

    widget_root_scene.render(renderer_base);

}



}

