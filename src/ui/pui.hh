#pragma once


#include "Windowing.hpp"
#include "Input.hpp"

#include "math/vecmat.hh"

#include "ui/render/renderer_ui.hh"

#include "conductor_common.hh"
#include "conductor_viewport.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_texture.hh"

#include "ui/ui_primitive.hh"
#include "ui/base.hh"

#include "ui/elements/string.hh"

namespace UI {


/** Physimos UI. */
class PUI {

public:

    UI::RendererUI renderer_ui;

    UI::Base* current_target = nullptr;

    UI::Base base_0;

    UI::Primitive prim_0;
    UI::Primitive prim_texture;

    UI::String string_prim_0;
    // UI::Primitive string_prim_0;

    PUI() 
        // : string_prim_0 {UI::Primitive("FFFF")}
    {

        // UI::font::init_font();
        UI::texture::init_static_color_textures();
        UI::texture::init_static_icon_textures();


        base_0.set_pos({100, 100});
        base_0.set_size({100, 100});

        // renderer_ui = UI::RendererPrimitive();
        // renderer_ui.set_window_info({},{});
        
        // prim_0.render
        prim_0.set_w("200x");
        prim_0.set_h("200x");
        prim_0.set_x("<400x");
        prim_0.set_y("_100x");

        prim_texture.set_w("20x");
        prim_texture.set_h("20x");
        prim_texture.set_x("<20x");
        prim_texture.set_y("_20x");
        prim_texture.set_texture(texture::get_icon(Icon::Right));

        string_prim_0.set_string("HOLA");

        // string_prim_0.set_w("200x");
        // string_prim_0.set_h("50x");

        // string_prim_0.str_setFontSize(FontSize::f18);
        // string_prim_0.str_setString("asd");
        // string_prim_0.str_setFontSize(FontSize::f18);
        // string_prim_0.str_setString("asd");
        // string_prim_0.str_setFontSize(FontSize::f24);
        // string_prim_0.set_x("<400x");
        // string_prim_0.set_y("_10x");

        // prim_0.set_color({});
    };

    void set_window_info(f2 size, f2 scale){
        renderer_ui.set_window_info(size, scale);
    }

    bool has_target(){

        return current_target != nullptr ? true : false;

    }

    bool has_grabbed_target(){

        if(current_target != nullptr)
            return current_target->mouse_is_down();

        return false;
    }

    /** Queries the ui for a Base matching input pos. Caches the found base as current target. */
    bool try_find_new_target(f2 cursor_pos_win_sane){
        
        BaseQuery base_query = base_0.containsPoint(cursor_pos_win_sane);
        
        if(base_query.success){
            current_target = &base_0;
            current_target->set_hover();
            return true;
        }

        // clear current target
        if(current_target != nullptr){
            current_target->mouse_up();
            current_target->unset_hover();
            current_target = nullptr;
        }

        return false;
    }

    void hover_current_target(){
        if(current_target != nullptr)
            current_target->set_hover();
    }
    void event_mouse_down_current_target(){
        if(current_target != nullptr)
            current_target->mouse_down();
    }
    void event_mouse_up_current_target(){
        if(current_target != nullptr)
            current_target->mouse_up();
    }
    void move_event(f2 cursor_delta){
        if(current_target != nullptr){
            
            if(current_target->mouse_is_down()){
                Box box = current_target->get_box();
                box.pos += cursor_delta;
                current_target->set_box(box);
            }
        }
    }


    BaseQuery try_find_base(f2 cursor_pos_win_sane){

        // Manually query each available base objects for now
        BaseQuery base_query = base_0.containsPoint(cursor_pos_win_sane);
        if(base_query.success)
            return base_query;
        
        return BaseQuery();
    }

    void render(){

        base_0.render(renderer_ui);

        prim_0.render(renderer_ui);
        prim_texture.render(renderer_ui);

        // string_prim_0.render(renderer_ui);
        string_prim_0.render_string(renderer_ui);
    }


};


}
