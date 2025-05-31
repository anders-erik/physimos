#include <iostream>

#include "phont/phont.hh"
#include "math/vecmat.hh"

#include "math/transform.hh"

#include "scene2D.hh"
#include "subscene2D.hh"

namespace scene {


QuadS2D & SubScene2D::get_quad(){
    return quad;
}



// QUAD CALLS
void SubScene2D::set_box(f2 pos, f2 size){
    quad.set_box(pos, size);
}
bool SubScene2D::contains_cursor(f2 pos_scene){
    return quad.contains_cursor(pos_scene);
}
f2 SubScene2D::get_normalized_from_point(f2 pos_scene){
    return quad.get_normalized_from_point(pos_scene);
}
m3f3 SubScene2D::get_matrix(){
    return quad.get_matrix();
}
opengl::RenderContextQuadS2D& SubScene2D::get_rendering_context(){
    return quad.get_rendering_context();
}
void SubScene2D::set_texture_id(unsigned int id){
    quad.set_texture_id(id);
}



ShapeS2D& SubScene2D::add_shape(Shape& shape){
    return scene.add_shape(shape);
}
void SubScene2D::add_quad(scene::QuadS2D& quad){
    scene.add_quad(quad);
}
void SubScene2D::update(){
    scene.update();
}
unsigned int SubScene2D::render(){
    return scene.render_to_texture();
}
void SubScene2D::handle_scroll(float delta){
    scene.handle_scroll(delta);
}
void SubScene2D::handle_pointer_move(scene::PointerMovement2D pointer_movement){
    scene.handle_pointer_move(pointer_movement);
}
void SubScene2D::handle_pointer_click(PointerClick2D pointer_click){
    scene.handle_pointer_click(pointer_click);
}

}
