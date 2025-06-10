#pragma once

#include <string>
#include <vector>

#include "math/vecmat.hh"
#include "math/box2D.hh"

#include "ui/ui_shader_texture.hh"
#include "ui/ui_shader_color.hh"
#include "ui/ui_shader_string.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_font.hh"

namespace UI {

// forward declare
class Base; 
class RendererBase;


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


protected:

    // UI::Box box;
    Box2D box;

    bool flag_hover = false;
    bool flag_click = false;

    unsigned int rgba_color = 0x000000ff;

public:

    Base() = default;


    // Box

    void set_pos(f2 pos);
    void set_size(f2 size);
    void set_box(const Box2D& box);
    Box2D& get_box();
    m4f4 get_M_m_s();
    Base* containsPoint(f2 point);

    // Color
    void set_rgba_color(unsigned int rgba_color);
    unsigned int get_rgba_color() const;
    f4 get_f4_color() const;

    // Events

    void set_hover();
    void unset_hover();
    bool is_hovered() const;

    void click();
    void unclick();
    bool is_clicked() const;

    void scroll(float delta);

    void clear_state_flags();


};


}


