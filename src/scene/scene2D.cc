#include <iostream>

#include "lib/str.hh"
#include "lib/print.hh"

#include "phont/phont.hh"
#include "math/vecmat.hh"
#include "math/transform.hh"

#include "scene2D.hh"
#include "subscene2D.hh"

namespace scene {




Scene2D::Scene2D(f2 _window_size) 
    : framebuffer { opengl::TextureFrameBufferMultisample(_window_size.to_i2(), 4) }
{

    set_window_size(_window_size);

    // FRAME INIT
    frame_M_m_s.x.x = 2.0f;
    frame_M_m_s.y.y = 1.0f;
    frame_M_m_s.x.z = 7.0f;
    frame_M_m_s.y.z = 6.0f;


    camera.set_window_size_px(window_size_f);
    camera.set_width(10.0f);
    camera.set_zoom_factor(1.2f);
    // camera.pan({ -400.0f, -300.0f}); // Half of window size
    camera.pan({ 0.0f, 0.0f});

}

void Scene2D::set_tag_id(size_t id){
    tag_id = id;
}

size_t Scene2D::get_tag_id(){
    return tag_id;
}


f2 Scene2D::get_window_size(){
    return window_size_f;
}

void Scene2D::set_window_size(f2 size){
    
    window_size_f = size;
    camera.set_window_size_px(size);

    // Subscenes do not need to be updated as their quads are fixed
    // for( scene::SubScene2D& subscene : subscenes){
    //     scene::Scene2D* subscene_scene = BC::get_scene(subscene.scene_tag.id);
    //     subscene_scene->set_window_size(size);
    // }

}

QuadS2D* Scene2D::try_match_cursor_to_quad(f2 pos_scene)
{

    for(QuadS2D& quad : quads){

        if(quad.contains_cursor(pos_scene))
            return &quad;

    }

    return nullptr;
}


f2 Scene2D::normalized_to_scene_conversion(f2 normalized)
{
    return {normalized.x * window_size_f.x, normalized.y * window_size_f.y};
}


Scene2D * Scene2D::try_find_current_scene(f2 normalized)
{
    f2 pos_current_scene = normalized_to_scene_conversion(normalized);

    for(scene::SubScene2D& subscene : subscenes){
    
        // Recursive match
        if(subscene.quad.contains_cursor(pos_current_scene)){

            subscene_current_target = &subscene;

			scene::Scene2D* subscene_scene = BC::get_scene(subscene.scene_tag.id);

            f2 pos_subscene_normalized = subscene.quad.get_normalized_from_point(pos_current_scene);

            return subscene_scene->try_find_current_scene(pos_subscene_normalized);

		}
    
    }

    // Found no subscene that captured the cursor
    subscene_current_target = nullptr;

    // No subscenes that matched the cursor position
    return this;
}


void Scene2D::handle_pointer_move(PointerMovement2D pointer_movement){

    // update cursor position in scene
    cursor_pos_normal = pointer_movement.pos_curr;
    cursor_pos_scene = camera.normalized_to_scene_coords(cursor_pos_normal);

    f2 cursor_pos_scene_prev = camera.normalized_to_scene_coords(pointer_movement.pos_prev);


    for(scene::SubScene2D& subscene : subscenes){
    
        // Check if position before move is within subscene
        if(subscene.quad.contains_cursor(cursor_pos_scene_prev)){

            PointerMovement2D subscene_pointer_move = {
                subscene.quad.get_normalized_from_point(cursor_pos_scene_prev),
                subscene.quad.get_normalized_from_point(cursor_pos_scene),
            };

            scene::Scene2D* subscene_scene = BC::get_scene(subscene.scene_tag.id);
            subscene_scene->handle_pointer_move(subscene_pointer_move);

            // Event is handled by subscene
            return;
            
        }
    
    }

    // As no other subscenes have captured the movement, we now can handle the event logic

    // Get move deltas in current scene
    f2 delta_normalized = pointer_movement.pos_curr - pointer_movement.pos_prev;
    f2 delta_scene = camera.normalized_to_scene_delta(delta_normalized);

    if(panable)
        camera.pan(delta_scene);
    

    QuadS2D* quad_ptr = try_match_cursor_to_quad(cursor_pos_scene);
    if(quad_ptr == nullptr)
        quad_current_hover = nullptr;
    else
        quad_current_hover = quad_ptr;

}

void Scene2D::handle_pointer_click(PointerClick2D pointer_click){
    
    // Update scene cursor
    cursor_pos_normal = pointer_click.pos_scene_normal;
    cursor_pos_scene = camera.normalized_to_scene_coords(cursor_pos_normal);


    for(scene::SubScene2D& subscene : subscenes){
    
        // Check if position before move is within subscene
        if(subscene.quad.contains_cursor(cursor_pos_scene)){

            PointerClick2D subscene_pointer_click = {
                subscene.quad.get_normalized_from_point(cursor_pos_scene),
                pointer_click.button_event,
            };

            scene::Scene2D* subscene_scene = BC::get_scene(subscene.scene_tag.id);
            subscene_scene->handle_pointer_click(subscene_pointer_click);

            // Event is handled by subscene
            return;
            
        }
    
    }


    window::MouseButtonEvent button_event = pointer_click.button_event;

    // Toggle Pan
    if(!panable && button_event.is_middle_down())
        panable = true;
    else if(panable && button_event.is_middle_up())
        panable = false;
    


    // Left Click
    if(button_event.is_left_down()){

        quad_current_selected = try_match_cursor_to_quad(cursor_pos_scene);
        if(quad_current_selected != nullptr){
            print("New quad selected: ");
            quad_current_selected->get_name().println();
        }
    
    }
}

void Scene2D::handle_scroll(float delta){

    for( scene::SubScene2D& subscene : subscenes){
    //     if(subscene.quad.contains_cursor();
    //     scene::Scene2D* subscene_scene = BC::get_scene(subscene.scene_tag.id);
        
    }
    
    camera.zoom(delta);
}




void Scene2D::add_quad(scene::QuadS2D& quad){

    // renderer2D.create_context_quad_t(quad.get_rendering_context(), quad.get_verts());
    quads.push_back(quad);
    
}


ShapeS2D& Scene2D::add_shape(Shape& shape){

    // scene::ShapeS2D new_shape_S2D = scene::ShapeS2D{shape};

    // renderer2D.create_shape_context_t(new_shape_S2D.render_context, new_shape_S2D.verts_render);

    if(shape.is(shape_t::point)){
        scene::ShapeS2D& new_shape_S2D = points.emplace_back(shape);
        renderer2D.create_shape_context_t(new_shape_S2D.render_context, new_shape_S2D.verts_render);
        return new_shape_S2D;
    }
    else if(shape.is(shape_t::line)){
        scene::ShapeS2D& new_shape_S2D = lines.emplace_back(shape);
        renderer2D.create_shape_context_t(new_shape_S2D.render_context, new_shape_S2D.verts_render);
        return new_shape_S2D;
    }
    else {
        scene::ShapeS2D& new_shape_S2D = shapes.emplace_back(shape);
        renderer2D.create_shape_context_t(new_shape_S2D.render_context, new_shape_S2D.verts_render);
        return new_shape_S2D;
    }

}

SubScene2D* ss;

// SubScene2D& Scene2D::add_subscene(f2 pos_scene, f2 size_scene){
// SubScene2D* add_subscene(f2 pos_scene, f2 size_scene){
SubScene2D& Scene2D::add_subscene(f2 pos_scene, f2 size_scene){
    
    scene::SubScene2D& new_subscene = subscenes.emplace_back(size_scene);
	new_subscene.quad.set_box(pos_scene, size_scene);
    
    // ss = new SubScene2D(size_scene);
    // return ss;
    // return *ss;
    // return subscenes.emplace_back(size_scene);
    return new_subscene;
}


void Scene2D::update(){

    // for(scene::ShapeS2D& _quad : quads)
    //     _quad.transform_2d.set_matrix_model();

    // for(scene::ShapeS2D& point : points)
    //     point.transform_2d.set_matrix_model();

    // for(scene::ShapeS2D& line : lines)
    //     line.transform_2d.set_matrix_model();

}


void Scene2D::render_subscene_textures(){

    for( scene::SubScene2D& subscene : subscenes){
        scene::Scene2D* subscene_scene = BC::get_scene(subscene.scene_tag.id);
        // recursively render descendants before itself
        subscene_scene->render_subscene_textures();
        subscenes[0].quad.set_texture_id(subscene_scene->render_to_texture());
    }

}


void Scene2D::render_to_window(){

    camera.set_width(window_size_f.x);

    render();
}


unsigned int Scene2D::render_to_texture(){

    render_subscene_textures();

    framebuffer.multisample_fbo_bind();
    framebuffer.multisample_fbo_clear_red();

    render();


    framebuffer.blit();

    return framebuffer.get_resolved_texture();
}

void Scene2D::render(){

    renderer2D.activate();
    // renderer2D.set_camera(camera.transform.matrix);
    renderer2D.set_camera(camera.get_matrix());

    
    for(scene::QuadS2D& quad : quads)
        renderer2D.render_quad(quad);

    for(scene::ShapeS2D& point : points){
        renderer2D.set_model(point.get_matrix());
        renderer2D.render_point(point.render_context);
    }

    for(scene::ShapeS2D& line : lines){
        renderer2D.set_model(line.get_matrix());
        renderer2D.render_line(line.render_context);
    }

    for( scene::SubScene2D& subscene : subscenes){
        renderer2D.render_quad(subscene.quad);
    }
    // Highlight subscene that is capturing cursor
    if(subscene_current_target != nullptr)
		renderer2D.render_frame(subscene_current_target->quad.get_matrix());

    
    // FRAMES
    renderer2D.render_frame(frame_M_m_s);

    if(quad_current_hover != nullptr)
        renderer2D.render_frame(quad_current_hover->get_matrix());
    if(quad_current_selected != nullptr)
        renderer2D.render_frame(quad_current_selected->get_matrix());


}




void Scene2D::handle_input(window::InputEvent event){

    if(event.type == window::EventType::MouseButton){

        // PAN
        if(event.mouse_button.button == window::MouseButton::Middle) {
    
            if(event.mouse_button.action == window::ButtonAction::Press)
                panable = true;
            else if(event.mouse_button.action == window::ButtonAction::Release)
                panable = false;

        }

    }
    else if(event.type == window::EventType::MouseScroll){

        

    }

    
    if(event.type == window::EventType::MouseMove){
    
        // event.mouse_movement.cursor.print();

        // Scene can be panned
        // if(panable)
        //     camera.pan(event.mouse_movement.delta.sane);

        
        // camera.set_cursor_pos(event.mouse_movement.pos_px, event.mouse_movement.pos_norm);
        // cursor_scene.set_cursor_pos(  event.cursor.sane, 
        //                                 event.cursor.normalized,
        //                                 camera.get_box()
        // );

    }



    if(event.key_stroke.key == window::Key::p && event.key_stroke.action == window::ButtonAction::Press){
        // quad.transform_2d.matrix.print();
        // camera.transform.matrix.print();

        // std::cout << "camera.transform.pos   = " << camera.transform.pos.x << " " << camera.transform.pos.y << std::endl;
        // std::cout << "camera.transform.scale = " << camera.transform.scale.x << " " << camera.transform.scale.y << std::endl;

        // std::cout << "camera.view_width = " << camera.view_width  << std::endl;

        camera.print();
        

        // std::cout << "camera.scene_per_sane = " << camera.scene_per_sane << std::endl;

        // std::cout << "camera.window_size_px = " << camera.window_size_px.x << " " << camera.window_size_px.y << std::endl;

        // std::cout << "camera.cursor_viewport_px    = " << camera.cursor_viewport_px.x << " " << camera.cursor_viewport_px.y << std::endl;
        // std::cout << "camera.cursor_viewport_norm  = " << camera.cursor_viewport_norm.x << " " << camera.cursor_viewport_norm.y << std::endl;
        // std::cout << "camera.cursor_viewport_scene = " << camera.cursor_viewport_scene.x << " " << camera.cursor_viewport_scene.y << std::endl;
        
        // std::cout << "cursor_scene.scene = " << cursor_scene.scene.x << " " << cursor_scene.scene.y << std::endl;
        
        // scene_2d.camera_2d.transform_2d.matrix.print();
        // std::cout << "panable = " << panable << std::endl;
        
    }
}


void Scene2D::print_info(){

    std::cout << "Scene:" << std::endl;

    cursor_pos_scene.print("Cursor [scene]:");
    cursor_pos_normal.print("Cursor [norm]:");
}

}
