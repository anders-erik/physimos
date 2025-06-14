#include <iostream>

#include "scene2D.hh"

#include "quadS2D.hh"

#include "manager.hh"


namespace scene {


const size_t QuadS2D::get_id() const
{
    return id;
}

void QuadS2D::set_name(Str new_name)
{
    name = new_name;
}
const Str& QuadS2D::get_name() const
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
m3f3 QuadS2D::get_model_matrix() const
{
    return box.get_matrix_m3f3();
}

f2& QuadS2D::get_pos_mut()
{
    return box.pos;
}

f2& QuadS2D::get_size_mut()
{
    return box.size;
}


void QuadS2D::set_texture_id(unsigned int new_id)
{
    texture_id = new_id;
}
unsigned int QuadS2D::get_texture_id() const
{
    return texture_id;
}

size_t QuadS2D::get_object_id()
{
    return object_id;
}



bool QuadS2D::contains_cursor(f2 cursor_pos_scene_coords)
{
    return box.contains_point(cursor_pos_scene_coords);   
}

f2 QuadS2D::scene_to_quad_normalized(f2 cursor_pos_scene_coords)
{
    return box.to_normalized(cursor_pos_scene_coords);
}



void QuadS2D::set_bitmap_texture(unsigned int opengl_texture_id)
{
    type = T::Bitmap;
    this->texture_id = opengl_texture_id;
}

void QuadS2D::set_scene(const Scene2D* new_scene)
{
    type = T::Scene2D;
    object_id = new_scene->get_id();
}

bool QuadS2D::is_bitmap()
{
    return type == T::Bitmap ? true : false;
}

bool QuadS2D::is_scene2D()
{
     return type == T::Scene2D ? true : false;
}

void QuadS2D::update_texture()
{
    
}

}
