#include <iostream>

#include "phont/phont.hh"
#include "math/vecmat.hh"

#include "math/transform.hh"

#include "scene2D.hh"
#include "subscene2D.hh"

#include "scene/bc.hh"

namespace scene {
SubScene2D::SubScene2D(f2 framebuffer_size)
    // : scene {Scene2D(framebuffer_size)}
{
    auto tag_scene_pair = BC::new_scene(framebuffer_size);
    
    scene_tag = tag_scene_pair.XX;
    auto new_scene_opt = tag_scene_pair.YY;

    if(new_scene_opt.is_null())
        scene_tag.type = BC::none;

    // scene::Scene2D& new_scene = new_scene_opt.get_ref();
}



bool SubScene2D::tag_is_valid(){
    auto scene_opt =  BC::get_scene(scene_tag);
    return scene_opt.is_null() ? false : true;
}

QuadS2D & SubScene2D::get_quad(){
    return quad;
}

// Scene2D & SubScene2D::get_scene(){
//     return scene;
// }



// QUAD CALLS
// void SubScene2D::set_box(f2 pos, f2 size){
//     quad.set_box(pos, size);
// }
// bool SubScene2D::contains_cursor(f2 pos_scene){
//     return quad.contains_cursor(pos_scene);
// }
// f2 SubScene2D::get_normalized_from_point(f2 pos_scene){
//     return quad.get_normalized_from_point(pos_scene);
// }
// m3f3 SubScene2D::get_matrix(){
//     return quad.get_matrix();
// }
// opengl::RenderContextQuadS2D& SubScene2D::get_rendering_context(){
//     return quad.get_rendering_context();
// }
// void SubScene2D::set_texture_id(unsigned int id){
//     quad.set_texture_id(id);
// }



OptPtr<ShapeS2D> SubScene2D::add_shape(Shape& shape){
    
    auto scene_opt =  BC::get_scene(scene_tag);
    if(scene_opt.is_null() )
        return {};
    
    auto _scene = scene_opt.get_ref();

    return &_scene.add_shape(shape);
}

// void SubScene2D::add_quad(scene::QuadS2D& quad){
    
//     scene.add_quad(quad);
// }
// void SubScene2D::update(){
//     scene.update();
// }
// unsigned int SubScene2D::render(){
//     return scene.render_to_texture();
// }
// void SubScene2D::handle_scroll(float delta){
//     scene.handle_scroll(delta);
// }
// void SubScene2D::handle_pointer_move(scene::PointerMovement2D pointer_movement){
//     scene.handle_pointer_move(pointer_movement);
// }
// void SubScene2D::handle_pointer_click(PointerClick2D pointer_click){
//     scene.handle_pointer_click(pointer_click);
// }


}
