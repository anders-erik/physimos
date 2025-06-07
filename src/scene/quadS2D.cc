#include <iostream>

#include "quadS2D.hh"

#include "math/vecmat.hh"

#include "math/transform.hh"

namespace scene {

using namespace opengl;


void QuadS2D::set_id(size_t new_id){
    id = new_id;
}
size_t QuadS2D::get_id(){
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

m3f3 QuadS2D::get_matrix(){
    return M_m_s;
}

void QuadS2D::set_box(f2 pos, f2 size){
    box.pos = {pos.x, pos.y};
    box.size = {size.x, size.y};

    M_m_s.x.z = pos.x;
    M_m_s.y.z = pos.y;

    M_m_s.x.x = size.x;
    M_m_s.y.y = size.y;
}
Box2D QuadS2D::get_box(){
    return box;
}


void QuadS2D::set_texture_id(unsigned int id){
    // render_context.texture = id;
    render_context.set_texture_id(id);
}
opengl::RenderContextQuadS2D& QuadS2D::get_rendering_context(){
    return render_context;
}


bool QuadS2D::contains_cursor(f2 cursor_pos_scene_coords){
    // using cursor_p = cursor_pos_scene_coords;
    
    // Managable name
    f2 cursor_pos = cursor_pos_scene_coords;

    bool x_passed = cursor_pos.x > box.pos.x && cursor_pos.x < (box.pos.x + box.size.x);

    bool y_passed = cursor_pos.y > box.pos.y && (cursor_pos.y < (box.pos.y + box.size.y));

    if(x_passed && y_passed)
            return true;

    return false;
}

f2 QuadS2D::scene_to_quad_normalized(f2 cursor_pos_scene_coords){
    
    f2 normalized_coords;

    normalized_coords.x = (cursor_pos_scene_coords.x - M_m_s.x.z) / M_m_s.x.x;
    normalized_coords.y = (cursor_pos_scene_coords.y - M_m_s.y.z) / M_m_s.y.y;
        
    return normalized_coords;
}



}
