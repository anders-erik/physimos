#include <iostream>

#include "quadS2D.hh"


namespace scene {



void QuadS2D::set_id(size_t new_id)
{
    id = new_id;
}
size_t QuadS2D::get_id()
{
    return id;
}

void QuadS2D::set_name(Str new_name)
{
    name = new_name;
}
Str & QuadS2D::get_name()
{
return name;
}


void QuadS2D::set_box(f2 pos, f2 size){
    box.pos = pos;
    box.size = size;
}
Box2D QuadS2D::get_box()
{
    return box;
}
m3f3 QuadS2D::get_model_matrix()
{
    M_m_s = box.get_matrix_m3f3();
    return M_m_s;
}


void QuadS2D::set_texture_id(unsigned int new_id)
{
    texture_id = new_id;
}
unsigned int QuadS2D::get_texture_id()
{
    return texture_id;
}



bool QuadS2D::contains_cursor(f2 cursor_pos_scene_coords)
{
    return box.contains_point(cursor_pos_scene_coords);   
}

f2 QuadS2D::scene_to_quad_normalized(f2 cursor_pos_scene_coords)
{
    return box.to_normalized(cursor_pos_scene_coords);
}



}
