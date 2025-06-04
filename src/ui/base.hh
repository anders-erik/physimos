#pragma once

#include <string>
#include <vector>

#include "math/vecmat.hh"

#include "ui/ui_shader_texture.hh"
#include "ui/ui_shader_color.hh"
#include "ui/ui_shader_string.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_font.hh"

namespace UI {

// forward declare
class Base; 
class RendererUI;


typedef struct BaseQuery {
    bool success = false;
    Base* base = nullptr;
    
    BaseQuery() = default;
    BaseQuery(Base* base) : success {true}, base {base} {};
} BaseQuery;



struct Box {
    f2 pos;
    f2 size;
};



class Base {

    UI::Box box;

    bool flag_hover = false;
    bool flag_mouse_down = false;

public:

    Base() = default;


    void set_pos(f2 pos);
    void set_size(f2 size);
    void set_box(const UI::Box& box);
    UI::Box& get_box();
    m4f4 get_M_m_s();
    BaseQuery containsPoint(f2 point);


    void set_hover();
    void unset_hover();
    bool is_hovered();

    void mouse_down();
    void mouse_up();
    bool mouse_is_down();

    void render(RendererUI& renderer);

};


}


