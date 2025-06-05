
// #include "ui/pui.hh"
#include "pui.hh"

namespace UI {

PUI::PUI() 
{
    // renderer_ui = UI::RendererPrimitive();
    // renderer_ui.set_window_info({},{});

    // UI::font::init_font();
    UI::texture::init_static_color_textures();
    UI::texture::init_static_icon_textures();


    base_0.set_pos({100, 100});
    base_0.set_size({100, 100});

    new_quad_in_root_scene.set_pos({50, 50});
    new_quad_in_root_scene.set_size({50, 50});

    base_texture.set_pos({110, 50});
    base_texture.set_size({50, 50});

    prim_color.set_w("200x");
    prim_color.set_h("200x");
    prim_color.set_x("<400x");
    prim_color.set_y("_100x");

    prim_texture.set_w("20x");
    prim_texture.set_h("20x");
    prim_texture.set_x("<20x");
    prim_texture.set_y("_20x");
    prim_texture.set_texture(texture::get_icon(Icon::Right));


    prim_string.str_setFontSize(FontSize::f18);
    prim_string.str_setString("HOLA!");
    prim_string.set_x("<200x");
    prim_string.set_y("_20x");
    prim_string.set_color( {0.0f, 0.0f, 0.0f, 0.5f} );

};


void PUI::set_window_info(f2 size, f2 scale){
    renderer_ui.set_window_info(size, scale);
}

bool PUI::has_target(){

    if(has_grabbed_target())
        return true;
    if(targeted_base != nullptr)
        return true;
    return false;

}

bool PUI::has_grabbed_target(){

    return grabbed_base == nullptr ? false : true;

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

    renderer_ui.draw_base(base_0);
    renderer_ui.draw_base(new_quad_in_root_scene);

    // renderer_ui.draw_base_texture(base_texture);
    base_texture.render_texture(renderer_ui);

    // PRIMITIVES
    prim_color.render(renderer_ui);
    prim_texture.render(renderer_ui);
    prim_string.render(renderer_ui);

}



}

