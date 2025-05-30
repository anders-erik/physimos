#include <iostream>

#include "phont/phont.hh"
#include "math/vecmat.hh"

#include "scene/scene2D.hh"
#include "math/transform.hh"
#include "scene2D.hh"

namespace scene {




Scene2D::Scene2D(f2 _window_size) 
    : framebuffer { opengl::TextureFrameBufferMultisample(_window_size.to_i2(), 4) }
{

    set_window_size(_window_size);

    // FRAME
    init_frame();

    camera.set_window_size_px(window_size_f);
    camera.set_width(10.0f);
    camera.set_zoom_factor(1.2f);
    // camera.pan({ -400.0f, -300.0f}); // Half of window size
    camera.pan({ 0.0f, 0.0f});

}


f2 Scene2D::get_window_size(){
    return window_size_f;
}

void Scene2D::set_window_size(f2 size){
    window_size_f = size;

    camera.set_window_size_px(size);
}


void Scene2D::init_frame(){
    frame.verts = ShapeS2D::generate_quad_line_frame_verts_0505(opengl::texture_get_color_coordinate(opengl::TextureColors::Green));
    renderer2D.create_wire_quad_context_t(frame.render_context, frame.verts);
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


void Scene2D::update(){

    // for(scene::ShapeS2D& _quad : quads)
    //     _quad.transform_2d.set_matrix_model();

    // for(scene::ShapeS2D& point : points)
    //     point.transform_2d.set_matrix_model();

    // for(scene::ShapeS2D& line : lines)
    //     line.transform_2d.set_matrix_model();

}


void Scene2D::render_window(){

    renderer2D.activate();
    // renderer2D.set_camera(camera.transform.matrix);
    renderer2D.set_camera(camera.get_matrix());

    // for(scene::ShapeS2D& _quad : quad_shapes){
    //     renderer2D.set_model(_quad.transform_2d.matrix);
    //     renderer2D.render_quad(_quad.render_context);
    // }

    for(scene::ShapeS2D& point : points){
        renderer2D.set_model(point.get_matrix());
        renderer2D.render_point(point.render_context);
    }

}


unsigned int Scene2D::render_to_texture(){

    framebuffer.multisample_fbo_bind();
    framebuffer.multisample_fbo_clear_red();


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

    // FRAME
    frame.M_m_s.x.x = 2.0f;
    frame.M_m_s.y.y = 1.0f;
    frame.M_m_s.x.z = 7.0f;
    frame.M_m_s.y.z = 6.0f;

    renderer2D.set_model(frame.M_m_s);
    renderer2D.render_frame(frame.render_context);


    framebuffer.blit();

    return framebuffer.get_resolved_texture();
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

void Scene2D::handle_pointer_move(PointerMovement2D pointer_movement){

    f2 delta_normalized = pointer_movement.pos_curr - pointer_movement.pos_prev;
    Box2D camera_box = camera.get_box();

    f2 delta_scene = {
        delta_normalized.x * camera_box.size.x, 
        delta_normalized.y * camera_box.size.y
    };

    if(panable)
        camera.pan(delta_scene);
    

    // PRINT POINTER POSITIONS

    // pointer_movement.pos_curr.print("Pointer [norm]:");
    
    // Current position of cursor in scene coordinates
    f2 pos_scene = {
        camera_box.pos.x + pointer_movement.pos_curr.x * camera_box.size.x, 
        camera_box.pos.y + pointer_movement.pos_curr.y * camera_box.size.y
    };

    // pos_scene.print("Pointer [scene]:");
}

void Scene2D::handle_pointer_click(PointerClick2D pointer_click){

    window::MouseButtonEvent button_event = pointer_click.button_event;

    if(!panable && button_event.is_middle_down())
        panable = true;
    else if(panable && button_event.is_middle_up())
        panable = false;
    
}

void Scene2D::handle_scroll(float delta){
    camera.zoom(delta);
}


void Scene2D::print(){

    std::cout << "Scene:" << std::endl;

    
    
}

}
