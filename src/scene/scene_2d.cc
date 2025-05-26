#include <iostream>

#include "phont/phont.hh"
#include "math/vecmat.hh"

#include "scene/scene_2d.hh"
#include "math/transform.hh"

namespace scene {


void CursorContext2D::set_cursor_pos(f2 pos_px, f2 pos_norm, Box2D camera_box){
    viewport_sane = pos_px;
    viewport_norm = pos_norm;

    scene.x = camera_box.pos.x + (viewport_norm.x - 0.5) * camera_box.size.x ;
    scene.y = camera_box.pos.y + (viewport_norm.y - 0.5) * camera_box.size.y ;
}



Scene2D::Scene2D(f2 _window_size) {

    set_window_size(_window_size);


    camera.set_window_size_px(window_size_f);
    camera.set_width(10.0f);
    camera.set_zoom_factor(1.2f);
    // camera.pan({ -400.0f, -300.0f}); // Half of window size
    camera.pan({ 0.0f, 0.0f});

}


void Scene2D::set_window_size(f2 size){
    window_size_f = size;

    camera.set_window_size_px(size);
}


void Scene2D::add_quad(opengl::ShapeS2D& quad_){
    renderer2D.create_context_quad(quad_);
    quads.push_back(quad_);
}

void Scene2D::add_shape(Shape& shape){

    opengl::ShapeS2D new_shape_S2D = opengl::ShapeS2D{shape};

    renderer2D.create_context(new_shape_S2D);

    if(shape.is(shape_t::point))
        points.push_back(new_shape_S2D);
    else if(shape.is(shape_t::line))
        lines.push_back(new_shape_S2D);
    else
        shapes.push_back(new_shape_S2D);

}


void Scene2D::update(){

    for(opengl::ShapeS2D& _quad : quads)
        _quad.transform_2d.set_matrix_model();

    for(opengl::ShapeS2D& point : points)
        point.transform_2d.set_matrix_model();

}


void Scene2D::render_window(){

    renderer2D.activate();
    // renderer2D.set_camera(camera.transform.matrix);
    renderer2D.set_camera(camera.get_matrix());

    for(opengl::ShapeS2D& _quad : quads){
        renderer2D.set_model(_quad.transform_2d.matrix);
        renderer2D.render_quad(_quad.render_context);
    }

    for(opengl::ShapeS2D& point : points){
        renderer2D.set_model(point.get_matrix());
        renderer2D.render_point(point.render_context);
    }

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

        camera.zoom(event.mouse_scroll.delta);

    }

    
    if(event.type == window::EventType::MouseMove){
    
        event.mouse_movement.cursor.print();

        // Scene can be panned
        if(panable)
            camera.pan(event.mouse_movement.delta);

        
        // camera.set_cursor_pos(event.mouse_movement.pos_px, event.mouse_movement.pos_norm);
        cursor_context.set_cursor_pos(  event.mouse_movement.cursor.sane, 
                                        event.mouse_movement.cursor.normalized,
                                        camera.get_box()
        );

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
        
        std::cout << "cursor_context.scene = " << cursor_context.scene.x << " " << cursor_context.scene.y << std::endl;
        
        // scene_2d.camera_2d.transform_2d.matrix.print();
        // std::cout << "panable = " << panable << std::endl;
        
    }
}


}
