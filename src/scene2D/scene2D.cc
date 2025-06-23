#include <iostream>

#include "lib/str.hh"
#include "lib/print.hh"

#include "phont/phont.hh"
#include "math/vecmat.hh"
#include "math/transform.hh"

#include "scene2D/quadS2D.hh"

#include "scene2D/manager_2D.hh"
#include "scene2D/scene2D.hh"





Scene2D::Scene2D(f2 _framebuffer_size) 
{
    // For now: each scene always have both framebuffers available
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

opengl::TextureFBMS & Scene2D::get_FBMS()
{
    return framebuffer_multisample;
}

opengl::TextureFB & Scene2D::get_FB()
{
    return framebuffer;
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




Camera2D& Scene2D::get_camera()
{
    return camera;
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



void Scene2D::try_hover_quad()
{
    auto& q_manager = manager_2D->manager_quad2d;
    q_manager.clear_hovered();

    for(QuadS2D* quad : q_manager.get_quads_from_ids_mut(quad_ids))
    {
        if(quad->contains_cursor(cursor_pos_scene))
            return q_manager.set_hovered(quad->get_id());
    }
}


bool Scene2D::try_select_quad()
{
    auto& q_manager = manager_2D->manager_quad2d;

    for(QuadS2D* quad : q_manager.get_quads_from_ids_mut(quad_ids))
    {
        if(quad->contains_cursor(cursor_pos_scene))
        {
            q_manager.set_selected(quad->get_id());
            return true;
        }
    }
    return false;
}





QuadQuery Scene2D::
try_find_quad_in_viewbox(f2 viewbox_pos_normalized)
{
    auto& q_manager = manager_2D->manager_quad2d;
    set_cursor_pos(viewbox_pos_normalized);

    // q_manager.get_quads_mut(quad_ids);

    for(QuadS2D* quad_safe_ptr : q_manager.get_quads_from_ids_mut(quad_ids))
    {
        QuadS2D& quad = *quad_safe_ptr;

        if(quad.contains_cursor(cursor_pos_scene))
        {
            Box2D camera_viewbox = camera.get_viewbox();
            
            Box2D quadbox_in_scene_normal = Box2D::to_normalized_box(
                camera_viewbox, 
                quad.get_box()
            );

            return {quad.get_id(), quadbox_in_scene_normal};
        }

    }
    return {0, Box2D()};
}



InputResponse Scene2D::
handle_pointer_move(window::InputEvent& event)
{
    // Get move deltas in current scene
    f2 delta_normalized = event.mouse_movement.cursor_new.normalized - event.mouse_movement.cursor_prev.normalized;
    f2 delta_scene = camera.normalized_to_scene_delta(delta_normalized);

    auto& q_manager = manager_2D->manager_quad2d;

    if(quad_grab)
    {
        auto* quad_hovered = q_manager.get_hovered();
        if(quad_hovered == nullptr)
            return {};
        
        for(size_t quad_id : quad_ids)
        {
            if(quad_hovered->get_id() != quad_id)
                continue;
            Box2D box = quad_hovered->get_box();
            f2 new_pos = box.pos + delta_scene;
            f2 new_size = box.size;
            quad_hovered->set_box(new_pos, new_size);
            return {InputResponse::MOUSE_GRAB};
        }
        
        return {};
    }

    if(scene_grab)
    {
        camera.pan(delta_scene);
        return {InputResponse::MOUSE_GRAB};
    }
    
    return {};
}

InputResponse Scene2D::
handle_pointer_click(window::InputEvent& event)
{    
    window::MouseButtonEvent button_event = event.mouse_button;

    // PAN / MOVE
    if(button_event.is_middle_down())
    {
        if(event.modifiers.is_ctrl_shift())
        {
            bool selected = try_select_quad();
            if(selected)
            {
                quad_grab = true;
            }
            else
            {
                return {};
            }
        }
        else
        {
            scene_grab = true;
        }
        return {InputResponse::MOUSE_GRAB};
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

    return {};
}



InputResponse Scene2D::
handle_scroll(window::InputEvent& scroll_event)
{
    // Disable scroll during grab
    if(scene_grab || quad_grab)
    {
        return {InputResponse::MOUSE_GRAB};
    }

    // Resize quad
    if(scroll_event.modifiers.is_ctrl_shift())
    {
        float size_factor = 1.1;
        if(scroll_event.mouse_scroll.delta < 0)
            size_factor = 1 / size_factor;

        try_resize_hovered_quad(size_factor);

        return {};
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

    return {};
}




void Scene2D::push_quad_id(size_t quad_id)
{
    quad_ids.push_back(quad_id);
}

void Scene2D::push_quad(QuadS2D& quad)
{
    auto& q_manager = manager_2D->manager_quad2d;
    size_t quad_id = q_manager.push_quad(quad);
    quad_ids.push_back(quad_id);
}



ShapeS2D& Scene2D::push_shape(Shape& shape){

    // scene::ShapeS2D new_shape_S2D = scene::ShapeS2D{shape};

    // renderer2D.create_shape_context_t(new_shape_S2D.render_context, new_shape_S2D.verts_render);

    if(shape.is(shape_t::point)){
        ShapeS2D& new_shape_S2D = points.emplace_back(shape);
        renderer2D.create_shape_context_t(new_shape_S2D.render_context, new_shape_S2D.verts_render);
        return new_shape_S2D;
    }
    else if(shape.is(shape_t::line)){
        ShapeS2D& new_shape_S2D = lines.emplace_back(shape);
        renderer2D.create_shape_context_t(new_shape_S2D.render_context, new_shape_S2D.verts_render);
        return new_shape_S2D;
    }
    else {
        ShapeS2D& new_shape_S2D = shapes.emplace_back(shape);
        renderer2D.create_shape_context_t(new_shape_S2D.render_context, new_shape_S2D.verts_render);
        return new_shape_S2D;
    }

}



size_t Scene2D::add_subscene2D(size_t scene_id, f2 quad_pos)
{
    Scene2D* scene_query = manager_2D->search_scene_storage_2D(scene_id);

    if(scene_query == nullptr) return 0;

    Scene2D& scene = (Scene2D&) *scene_query;

    QuadS2D new_quad;
    new_quad.set_box( quad_pos, scene.get_framebuffer_size() );
    new_quad.set_name("scene_quad_test");
    new_quad.set_scene(&scene);
    new_quad.update_texture();

    auto& q_manager = manager_2D->manager_quad2d;
    size_t new_quad_id = q_manager.push_quad(new_quad);
    quad_ids.push_back(q_manager.push_quad(new_quad));

    return new_quad_id;
}





void Scene2D::try_resize_hovered_quad(float size_factor)
{
    auto& q_manager = manager_2D->manager_quad2d;

    auto* quad_hovered = q_manager.get_hovered();
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


void Scene2D::release_grabs()
{
    quad_grab = false;
    scene_grab = false;
}



