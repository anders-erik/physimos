#include <iostream>

#include "lib/str.hh"
#include "lib/print.hh"

#include "phont/phont.hh"
#include "math/vecmat.hh"
#include "math/transform.hh"

#include "scene/quadS2D.hh"

#include "scene2D.hh"
#include "subscene2D.hh"

namespace scene {




void Scene2D::clear_hovers()
{
    quad_manager.clear_hovered();
    subscene_current_hover = nullptr;
    // quad_current_hover = nullptr;
}
void Scene2D::clear_selections()
{
    quad_manager.clear_selection();
}
void Scene2D::clear_grab()
{
    panable = false;
}

Box2D Scene2D::get_window_box()
{
    return window_box;
}

Scene2D::Scene2D(f2 _window_size) 
    : framebuffer { opengl::TextureFrameBufferMultisample(_window_size.to_i2(), 4) }
{

    set_framebuffer_size(_window_size);

    // FRAME INIT
    frame_M_m_s.x.x = 2.0f;
    frame_M_m_s.y.y = 1.0f;
    frame_M_m_s.x.z = 7.0f;
    frame_M_m_s.y.z = 6.0f;

    set_zoom_factor(1.2f);

    camera.set_framebuffer_size(framebuffer_size_f);
    camera.set_width(10.0f);
    // camera.pan({ -400.0f, -300.0f}); // Half of window size
    camera.pan({ 0.0f, 0.0f});

}

void Scene2D::set_id(size_t id){
    this->id = id;
}
size_t Scene2D::get_id() const {
    return id;
}

void Scene2D::set_parent_id(size_t parent_id){
    this->parent_id = parent_id;
}
size_t Scene2D::get_parent_id(){
    return parent_id;
}


f2 Scene2D::get_framebuffer_size(){
    return framebuffer_size_f;
}

void Scene2D::set_framebuffer_size(f2 size)
{    
    framebuffer_size_f = size;
    camera.set_framebuffer_size(size);

    // Subscenes do not need to be updated as their quads are fixed
}

void Scene2D::set_camera_width(float width)
{
    camera.set_width(width);
}
void Scene2D::set_zoom_factor(float new_zoom_factor)
{
    zoom_factor = new_zoom_factor;
}


QuadS2D* Scene2D::try_match_cursor_to_quad(f2 pos_scene)
{
    for(size_t quad_id : quad_ids)
    {
        auto* quad = quad_manager.get_quad(quad_id);
        if(quad == nullptr)
            continue;
        
        if(quad->contains_cursor(pos_scene))
            return quad;

    }

    return nullptr;
}



Scene2D * Scene2D::try_find_target_scene(f2 normalized, Box2D window_box)
{
    Scene2D* result = this;

    this->window_box = window_box;

    // This updates the cursor position in scenes
    cursor_pos_normal = normalized;
    cursor_pos_scene = camera.normalized_to_scene_coords(normalized);


    for(scene::SubScene2D& subscene : subscenes){
    
        // Recursive match
        if(subscene.quad.contains_cursor(cursor_pos_scene)){

            subscene_current_hover = &subscene;

			scene::Scene2D* subscene_scene = ManagerScene::get_scene(subscene.scene_id);

            // The location of the cursor as measued in its own nrmalized coordinates
            f2 cursor_pos_in_quad_norm = subscene.quad.scene_to_quad_normalized(cursor_pos_scene);

            // creates a window box whose box-relationship is congruent to that of current camera & quad box
            Box2D subscene_window_box = window_box.new_congruent_subbox(camera.get_box(), subscene.quad.get_box());

            result = subscene_scene->try_find_target_scene(cursor_pos_in_quad_norm, subscene_window_box);
		}
        else {
            scene::Scene2D* subscene_scene = ManagerScene::get_scene(subscene.scene_id);
            subscene_scene->clear_hovers();
        }
    
    }

    quad_manager.clear_hovered();
    // Found no subscene that captured the cursor
    if(result == this)
    {
        subscene_current_hover = nullptr;

        // Try regular quad instead of subscene
        QuadS2D* quad_ptr = try_match_cursor_to_quad(cursor_pos_scene);
        // quad_current_hover = nullptr;

        if(quad_ptr != nullptr)
        {
            // quad_manager.clear_hovered();
            // quad_current_hover = quad_ptr;
            // quad_current_selected = quad_ptr;
            // quad_manager.set_selected(quad_ptr->get_quad_id());
            quad_manager.set_hovered(quad_ptr->get_quad_id());

            if(quad_ptr->is_scene2D())
            {
                auto* subscene_scene = ManagerScene::get_scene(quad_ptr->get_object_id());

                // The location of the cursor as measued in its own nrmalized coordinates
                f2 cursor_pos_in_quad_norm = quad_ptr->scene_to_quad_normalized(cursor_pos_scene);

                // creates a window box whose box-relationship is congruent to that of current camera & quad box
                Box2D subscene_window_box = window_box.new_congruent_subbox(camera.get_box(), quad_ptr->get_box());

                result = subscene_scene->try_find_target_scene(cursor_pos_in_quad_norm, subscene_window_box);
                
            }
        }
    }

    return result;
}


void Scene2D::handle_pointer_move(PointerMovement2D pointer_movement)
{
    // Get move deltas in current scene
    f2 delta_normalized = pointer_movement.pos_norm_curr - pointer_movement.pos_norm_prev;
    f2 delta_scene = camera.normalized_to_scene_delta(delta_normalized);

    if(panable)
    {
        camera.pan(delta_scene);
    }
    
}

void Scene2D::handle_pointer_click(PointerClick2D pointer_click){
    
    window::MouseButtonEvent button_event = pointer_click.button_event;

    // Toggle Pan
    if(!panable && button_event.is_middle_down())
        panable = true;
    else if(button_event.is_middle_up())
        panable = false;
    

    // Left Click
    if(button_event.is_left_down()){


        auto* quad_click_match = try_match_cursor_to_quad(cursor_pos_scene);
        if(quad_click_match != nullptr)
        {
            quad_manager.set_selected(quad_click_match->get_quad_id());

            print("New quad selected: ");
            quad_click_match->get_name().print_line();
        }
    
    }
}

void Scene2D::handle_scroll(float delta)
{
    // The raw set_width() will just scale the position, thus changing the cursor pos for user
    if(delta > 0)
    {
        camera.set_width(camera.get_width() / zoom_factor);
    }
    else
    {
        camera.set_width(camera.get_width() * zoom_factor);
    }

    // Move camera to match cursor position
    // TODO: Move this into a camera.zoom()-call?
    
    // The normalized cursor position is unaffected by camera size, so we get the position after zoom
    f2 new_cursor_pos_scene = camera.normalized_to_scene_coords(cursor_pos_normal);

    // pan the difference between pre and post position
    f2 scroll_induced_move = new_cursor_pos_scene - cursor_pos_scene;
    camera.pan(scroll_induced_move);    
}




void Scene2D::add_quad(scene::QuadS2D& quad)
{
    quad_ids.push_back(quad_manager.add_quad(quad));
}

size_t Scene2D::add_quad_default()
{
    scene::QuadS2D new_quad;
    new_quad.set_box({0.5f, 3.0f}, {5.0f, 1.5f});
    new_quad.set_bitmap_texture(0);

    size_t new_quad_id = quad_manager.add_quad(new_quad);
    quad_ids.push_back(new_quad_id);

    return new_quad_id;
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


SubScene2D& Scene2D::add_subscene(f2 pos_scene, f2 size_scene)
{
    scene::Scene2D* new_scene = ManagerScene::new_scene(size_scene);
    if(new_scene == nullptr)
        throw std::runtime_error("New subscene scene is nullptr");
    new_scene->set_parent_id(id);

    // Create quad with scene type and scene id
    scene::SubScene2D& new_subscene = subscenes.emplace_back();
    new_subscene.scene_id = new_scene->get_id();
	new_subscene.quad.set_box(pos_scene, size_scene);

    return new_subscene;
}

size_t Scene2D::add_subscene(Scene2D* new_scene)
{
    new_scene->set_parent_id(id);
    new_scene->add_quad_default();

    scene::QuadS2D new_quad;
    new_quad.set_box( {200.0f, 0.0f}, new_scene->get_framebuffer_size() );
    new_quad.set_name("scene_quad_test");
    new_quad.set_scene(new_scene);
    new_quad.update_texture();

    size_t quad_id = quad_manager.add_quad(new_quad);
    quad_ids.push_back(quad_id);

    return quad_id;
}


void Scene2D::update(){

    // for(scene::ShapeS2D& _quad : quads)
    //     _quad.transform_2d.set_matrix_model();

    // for(scene::ShapeS2D& point : points)
    //     point.transform_2d.set_matrix_model();

    // for(scene::ShapeS2D& line : lines)
    //     line.transform_2d.set_matrix_model();

}


void Scene2D::
render_subscene_textures()
{

    for(size_t quad_id : quad_ids)
    {
        auto* quad = quad_manager.get_quad(quad_id);
        if(quad == nullptr)
            continue;
        
        quad->update_texture();
    }

    for( scene::SubScene2D& subscene : subscenes){
        scene::Scene2D* subscene_scene = ManagerScene::get_scene(subscene.scene_id);
        // recursively render descendants before itself
        subscene_scene->render_subscene_textures();
        subscenes[0].quad.set_texture_id(subscene_scene->render_to_texture());
    }

}


void Scene2D::
render_to_window()
{
    // camera.set_width(window_size_f.x);

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

    
    // for(scene::QuadS2D& quad : quads)
    //     renderer2D.render_quad(quad);
    for(size_t quad_id : quad_ids)
    {
        scene::QuadS2D* quad_p = quad_manager.get_quad(quad_id);
        if(quad_p != nullptr)
            renderer2D.render_quad(*quad_p);
    }

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
    if(subscene_current_hover != nullptr)
		renderer2D.render_frame(subscene_current_hover->quad.get_model_matrix(), false, 2);

    
    // FRAMES
    renderer2D.render_frame(frame_M_m_s, false, 1);

    // Hovered quad frame
    auto* hovered_quad = quad_manager.get_hovered();
    if(hovered_quad != nullptr)
    {
        renderer2D.render_frame(hovered_quad->get_model_matrix(), false, 4);
    }

    // Selected quad frame
    auto* selected_quad = quad_manager.get_selected();
    if(selected_quad != nullptr)
    {
        renderer2D.render_frame(selected_quad->get_model_matrix(), true, 2);
    }

}



}
