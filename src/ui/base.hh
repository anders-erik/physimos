#pragma once


#include "math/vecmat.hh"
#include "math/box2D.hh"


namespace UI 
{


class Base {


protected:
public:

    // UI::Box box;
    Box2D box;

    bool flag_hover = false;
    bool flag_click = false;

    unsigned int rgba_color = 0x000000ff;


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


