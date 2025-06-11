#include <iostream>

#include "lib/str.hh"
#include "lib/print.hh"

#include "phont/phont.hh"
#include "math/vecmat.hh"
#include "math/transform.hh"

#include "scene/quadS2D.hh"

#include "scene/manager.hh"

#include "scene2D.hh"


namespace scene {





void Scene2D::clear_cursor_grab()
{
    scene_grab = false;
}


Box2D Scene2D::get_window_box()
{
    return window_box;
}

void Scene2D::try_grab_quad()
{
    auto* quad_hovered = quad_manager.get_hovered();
    if(quad_hovered != nullptr)
    {
        quad_grab = true;
    }
}

void Scene2D::grab_scene()
{
    scene_grab = true;
}

void Scene2D::release_grabs()
{
    quad_grab = false;
    scene_grab = false;
}

void Scene2D::try_resize_hovered_quad(float size_factor)
{
    auto* quad_hovered = quad_manager.get_hovered();
    if(quad_hovered != nullptr)
    {
        if(quad_hovered->is_bitmap())
        {
            Box2D current_quad_box = quad_hovered->get_box();
            f2 new_pos = current_quad_box.pos;
            f2 new_size = current_quad_box.size * size_factor;
            quad_hovered->set_box(new_pos, new_size);
        }
        if(quad_hovered->is_scene2D())
        {
            println("Scale scene quad");
        }
    }
}

Scene2D::Scene2D(f2 _framebuffer_size) 
{

    // For now each scene always have both framebuffers available
    framebuffer_multisample.reload(_framebuffer_size.to_i2(), 4);
    framebuffer.reload(_framebuffer_size.to_i2().x, _framebuffer_size.to_i2().y);


    set_framebuffer_size(_framebuffer_size);

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


bool Scene2D::
is_multisampled()
{
    return multisample_flag;
}


void Scene2D::
enable_multisample()
{
    multisample_flag = true;
}


void Scene2D::
disable_multisample()
{
    multisample_flag = false;
}


Str& Scene2D::get_name()
{
    return name;
}

void Scene2D::set_name(const Str& new_name)
{
    name = new_name;
}

f2 Scene2D::get_framebuffer_size(){
    return framebuffer_size_f;
}

void Scene2D::set_framebuffer_size(f2 size)
{    
    framebuffer_size_f = size;
    camera.set_framebuffer_size(size);

    // TODO: Reload actual framebuffer!

    // Subscenes do not need to be updated as their quad aspect ratios are fixed
}

void Scene2D::set_viewbox_width(float width)
{
    camera.set_width(width);
}

void Scene2D::set_zoom_factor(float new_zoom_factor)
{
    zoom_factor = new_zoom_factor;
}

void Scene2D::set_cursor_pos(f2 cursor_pos_normalized)
{
    cursor_pos_normal = cursor_pos_normalized;
    cursor_pos_scene = camera.normalized_to_scene_coords(cursor_pos_normal);
}

void Scene2D::set_window_box(Box2D new_window_box)
{
    window_box = new_window_box;
}


QuadS2D* Scene2D::try_match_cursor_to_quad(f2 pos_scene)
{
    for(QuadS2D& quad : quad_manager.get_quads_mut())
    {
        if(quad.contains_cursor(pos_scene))
            return &quad;
    }
    return nullptr;
}


void Scene2D::try_hover_quad()
{
    quad_manager.clear_hovered();

    QuadS2D* quad = try_match_cursor_to_quad(cursor_pos_scene);

    if(quad != nullptr)
        quad_manager.set_hovered(quad->get_id());
}

void Scene2D::try_select_quad()
{
    quad_manager.clear_selection();

    QuadS2D* quad = try_match_cursor_to_quad(cursor_pos_scene);

    if(quad != nullptr)
    {
        quad_manager.set_selected(quad->get_id());
        print("New quad selected: ");
        quad->get_name().print_line();
    }
}

Scene2D* Scene2D::try_find_window_subscene()
{
    QuadS2D* quad = try_match_cursor_to_quad(cursor_pos_scene);

    if(quad != nullptr && quad->is_scene2D())
    {
        auto* subscene  = ManagerScene::try_find_scene(quad->get_object_id());
        if(subscene == nullptr)
            return this;
        

        // Set cursor information for new targeted box
        // TODO: Turn window box of target to be a returned structure instead of scene-tracked?
        Box2D subscene_window_box = window_box.new_congruent_subbox(camera.get_box(), quad->get_box());
        subscene->set_window_box(subscene_window_box);

        // Cursor position is necessary for the scene to be able to handle envents
        f2 cursor_subscene_normalized = quad->scene_to_quad_normalized(cursor_pos_scene);
        subscene->set_cursor_pos(cursor_subscene_normalized);

        return subscene;
    }

    return this;
}


void Scene2D::handle_pointer_move(PointerMovement2D pointer_movement)
{
    // Get move deltas in current scene
    f2 delta_normalized = pointer_movement.pos_norm_curr - pointer_movement.pos_norm_prev;
    f2 delta_scene = camera.normalized_to_scene_delta(delta_normalized);

    if(quad_grab)
    {
        auto* quad_hovered = quad_manager.get_hovered();
        if(quad_hovered != nullptr)
        {
            Box2D box = quad_hovered->get_box();
            f2 new_pos = box.pos + delta_scene;
            f2 new_size = box.size;
            quad_hovered->set_box(new_pos, new_size);
        }
        return;
    }

    if(scene_grab)
    {
        camera.pan(delta_scene);
    }
    
}

void Scene2D::handle_pointer_click(PointerClick2D pointer_click)
{    
    window::MouseButtonEvent button_event = pointer_click.event.mouse_button;

    // PAN / MOVE
    if(button_event.is_middle_down())
    {
        if(pointer_click.event.modifiers.is_ctrl_shift())
            try_grab_quad();
        else
            grab_scene();
    }

    if(button_event.is_middle_up())
    {
        release_grabs();
    }


    // Left Click
    if(button_event.is_left_down())
    {
        try_select_quad();
    }
}

void Scene2D::handle_scroll(window::InputEvent scroll_event)
{
    // Resize quad
    if(scroll_event.modifiers.is_ctrl_shift())
    {
        float size_factor = 1.1;
        if(scroll_event.mouse_scroll.delta < 0)
            size_factor = 1 / size_factor;

        try_resize_hovered_quad(size_factor);

        return;
    }


    if(scroll_event.mouse_scroll.delta > 0)
    {
        float size_factor = 1 / zoom_factor;
        camera.zoom_cursor_fixed(size_factor, cursor_pos_normal);
    }
    else
    {
        float size_factor = zoom_factor;
        camera.zoom_cursor_fixed(size_factor, cursor_pos_normal);
    }  
}




void Scene2D::push_quad(scene::QuadS2D& quad)
{
    quad_manager.add_quad(quad);
}



ShapeS2D& Scene2D::push_shape(Shape& shape){

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


QuadS2D* Scene2D::add_subscene2D(size_t scene_id, f2 quad_pos)
{
    auto* scene = ManagerScene::try_find_scene(scene_id);
    scene->set_parent_id(id);

    scene::QuadS2D new_quad;
    new_quad.set_box( quad_pos, scene->get_framebuffer_size() );
    new_quad.set_name("scene_quad_test");
    new_quad.set_scene(scene);
    new_quad.update_texture();

    return quad_manager.add_quad(new_quad);
}





void Scene2D::
update()
{

}


void Scene2D::
render_subscene_textures()
{
    for(QuadS2D& quad : quad_manager.get_quads_mut())
        quad.update_texture();
}


void Scene2D::
render_to_window()
{
    // camera.set_width(window_size_f.x);

    render();
}


unsigned int Scene2D::render_to_texture()
{
    render_subscene_textures();

    if(multisample_flag)
    {
        framebuffer_multisample.multisample_fbo_bind();
        framebuffer_multisample.multisample_fbo_clear_red();

        render();

        framebuffer_multisample.blit();

        return framebuffer_multisample.get_resolved_texture();
    }
    else
    {
        framebuffer.framebuffer_bind();
        framebuffer.clear_with({0.5f, 0.5f, 0.5f, 1.0f});

        render();

        return framebuffer.get_texture_id();
    }
}


void Scene2D::render()
{
    renderer2D.activate();
    renderer2D.set_camera(camera.get_matrix());

    
    for(QuadS2D& quad : quad_manager.get_quads_mut())
    {
        renderer2D.render_quad(quad);
    }

    for(scene::ShapeS2D& point : points){
        renderer2D.set_model(point.get_matrix());
        renderer2D.render_point(point.render_context);
    }

    for(scene::ShapeS2D& line : lines){
        renderer2D.set_model(line.get_matrix());
        renderer2D.render_line(line.render_context);
    }
    
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
