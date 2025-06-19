#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "lib/string.hh"

#include "rend/shader.hpp"
#include "file_format/bmp_loader.hpp"

#include "ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/ui_font.hh"

#include "rend/pui/renderer_base.hh"

#include "base.hh"

namespace UI {



void Base::set_box(const Box2D & box){
    this->box = box;
}
Box2D& Base::get_box(){
    return box;
}
m4f4 Base::get_M_m_s(){
    m4f4 M_m_s;
    M_m_s.x.w = box.pos.x;
    M_m_s.y.w = box.pos.y;
    M_m_s.x.x = box.size.x;
    M_m_s.y.y = box.size.y;
    return M_m_s;
}

void Base::set_pos(f2 pos)
{
    box.pos = pos;
}
void Base::set_size(f2 size)
{
    box.size = size;
}


Base* Base::containsPoint(f2 point) 
{
    return box.contains_point(point) ? this : nullptr;
}

void Base::set_rgba_color(unsigned int rgba_color)
{
    this->rgba_color = rgba_color;
}

unsigned int Base::get_rgba_color() const
{
    return rgba_color;
}

f4 Base::get_f4_color() const
{
    float A = ((float) ((rgba_color << 24) >> 24)) / 256.0f;
    float B = ((float) ((rgba_color << 16) >> 24)) / 256.0f;
    float G = ((float) ((rgba_color <<  8) >> 24)) / 256.0f;
    float R = ((float) ((rgba_color <<  0) >> 24)) / 256.0f;

    return {R, G, B, A};
}


void Base::set_hover(){
    flag_hover = true;
}
void Base::unset_hover(){
    flag_hover = false;
}

bool Base::is_hovered() const
{
    return flag_hover;
}


void Base::click()
{
    flag_click = true;
}
void Base::unclick()
{
    flag_click = false;
}
bool Base::is_clicked() const
{
    return flag_click;
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
    flag_click = false;
    flag_hover = false;
}

    



}
