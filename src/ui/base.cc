#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "lib/string.hh"

#include "render/shader.hpp"
#include "file_format/bmp_loader.hpp"

#include "ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/ui_font.hh"

#include "ui/render/renderer_ui.hh"

#include "base.hh"

namespace UI {



void Base::set_box(const UI::Box & box){
    this->box = box;
}
UI::Box& Base::get_box(){
    return box;
}
m4f4 Base::get_M_m_s(){
    m4f4 M_m_s;
    M_m_s.x.w = box.pos.x;
    M_m_s.y.w = box.pos.y;
    M_m_s.x.x = box.size.x;
    M_m_s.y.y = box.size.x;
    return M_m_s;
}

void Base::set_pos(f2 pos){

    box.pos = pos;

}
void Base::set_size(f2 size){
    box.size = size;
}


Base* Base::containsPoint(f2 point) {

    bool x_pass = point.x > box.pos.x && point.x < box.pos.x + box.size.x;
    bool y_pass = point.y > box.pos.y && point.y < box.pos.y + box.size.y;

    // return x_pass && y_pass ? BaseQuery(this) : BaseQuery();
    return x_pass && y_pass ? this : nullptr;

}


void Base::set_hover(){
    flag_hover = true;
}
void Base::unset_hover(){
    flag_hover = false;
}

bool Base::is_hovered(){
    return flag_hover;
}


void Base::mouse_down()
{
    flag_mouse_down = true;
}
void Base::mouse_up()
{
    flag_mouse_down = false;
}
bool Base::mouse_is_down()
{
    return flag_mouse_down;
}

void Base::scroll(float delta)
{
    float scaled_delta = delta * 10;

    box.size.x += scaled_delta;
    box.size.y += scaled_delta;

    if(box.size.x < 10.0f || box.size.y < 10.0f)
    {
        box.size.x -= scaled_delta;
        box.size.y -= scaled_delta;
    }

}

void Base::clear_state_flags(){
    flag_mouse_down = false;
    flag_hover = false;
}

    



}
