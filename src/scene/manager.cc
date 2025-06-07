#include <list>

#include "lib/log.hh"

#include "window/auxevent.hh"

#include "scene2D.hh"

#include "manager.hh"

namespace ManagerScene {


static size_t id_count = 2; // Global index count

static bool is_grabbing_a_scene = false;
static f2 window_size = {0.0f, 0.0f};

static struct SceneState {
    const size_t root_id = 1; // root scene is created during init; is indestructable; id==1;
    size_t window_id = 1; // the scene renedered to the main framebuffer
    size_t active_id = 1; // Currently selected scene by mouse click event; recieves non mouse input events
    size_t cursor_target_id = 1; // scene curently hovered by cursor - recieves mouse click and movement events
} state;

// std::vector<scene::Scene2D> scenes;
static std::list<scene::Scene2D> scenes;


/** 
    Returns a unique, not equal to 0 or 1, unsigned integer.
    An id of 0 is globally understood as having no object asssociated with it; ids of 1 is always the fixed root scene.
 */
size_t new_unique_id()
{
    ++id_count;

    if      (id_count == 0)
        id_count += 2;
    else if (id_count == 1)
        id_count += 1;
    
    return id_count;
}

scene::Scene2D * try_find_scene(size_t id){

    for(scene::Scene2D& scene : scenes)
        if(scene.get_id() == id)
            return &scene;
    
    return nullptr;

}

scene::Scene2D* new_scene(scene::Scene2D & _scene){
    
    size_t new_id = new_unique_id();

    _scene.set_id(new_id);

    scenes.push_back( _scene );


    scene::Scene2D* new_scene = try_find_scene(new_id);

    return new_scene;

}

scene::Scene2D* new_scene(f2 framebuffer_size)
{
    scene::Scene2D& new_scene = scenes.emplace_back( framebuffer_size );

    size_t new_id = new_unique_id();
    new_scene.set_id(new_id);
    
    return &new_scene;
}

scene::Scene2D* init(f2 _window_size)
{
    if(scenes.size() > 1)
    {
        throw std::runtime_error("Can't init the scene manager twice");
    }

    window_size = _window_size;
    
    scene::Scene2D& root_scene = scenes.emplace_back( window_size );

    root_scene.set_id(state.root_id);

    state.window_id = state.root_id;
    state.active_id = state.root_id;
    state.cursor_target_id = 0;

    return &root_scene;
}

scene::Scene2D* get_root_scene()
{
    scene::Scene2D* root_scene = try_find_scene(state.root_id);
    // debug info if root scene, which should NEVER be null, is in fact null
    if(root_scene == nullptr)
			throw std::runtime_error("Root scene is nullptr");

    return root_scene;
}

scene::Scene2D* get_scene(size_t id)
{
    return try_find_scene(id);
}

void update_current_target(f2 cursor_pos_window_normalized)
{
    // maintain most recent target
    if(is_grabbing_a_scene)
        return;

    f2 window_pos = {0.0f, 0.0f}; // window scene is always filling the whole window
    scene::Scene2D* current_scene_target = get_root_scene()->try_find_target_scene(cursor_pos_window_normalized, { window_pos, window_size });

    state.cursor_target_id = current_scene_target->get_id();

}

bool has_grabbed_target()
{
    return is_grabbing_a_scene;
}

void clear_current_target()
{
    // clear all scenes highlighting
    for(scene::Scene2D& scene : scenes)
    {
        scene.clear_hovers();
        scene.clear_grab();
    }

    scene::Scene2D* cursor_target_scene = get_scene(state.cursor_target_id);
    
    state.cursor_target_id = 0;
}

scene::Scene2D * get_current_target()
{
    return get_scene(state.cursor_target_id);
}

void event_scroll(window::InputEvent & event)
{   
    window::MouseScrollEvent& scroll_event = event.mouse_scroll;

    scene::Scene2D* current_target = ManagerScene::get_current_target();
		if(current_target != nullptr)
			current_target->handle_scroll(scroll_event.delta);

}

void event_move(window::InputEvent & event)
{
    // window::MouseMoveEvent& mouse_movement = event.mouse_movement;
	window::CursorPosition& cursor_new = event.cursor;
	window::CursorPosition& cursor_prev = event.mouse_movement.cursor_prev;

    scene::Scene2D* current_target = ManagerScene::get_current_target();
		if(current_target == nullptr)
			return;

    scene::PointerMovement2D scene_pointer_move;
    scene_pointer_move.pos_prev = cursor_prev.normalized;
    scene_pointer_move.pos_curr = cursor_new.normalized;

    scene::Scene2D* root_scene = ManagerScene::get_root_scene();
    root_scene->handle_pointer_move(scene_pointer_move);
}

void event_mouse_button(window::InputEvent & event)
{
    window::CursorPosition& cursor = event.cursor;
	window::MouseButtonEvent mouse_button_event = event.mouse_button;
    
	scene::Scene2D* root_scene = ManagerScene::get_root_scene();
	
	scene::PointerClick2D pointer_click = {
		cursor.normalized,
		mouse_button_event
	};

	root_scene->handle_pointer_click(pointer_click);

    if(mouse_button_event.is_middle_down())
    {
        is_grabbing_a_scene = true;
    }
    else if(mouse_button_event.is_middle_up())
    {
        is_grabbing_a_scene = false;
        clear_current_target();
    }

}

void event_keystroke(window::InputEvent & event)
{
    using namespace window;

    if(event.keystroke.is(Key::p)){

        if(event.keystroke.is(ButtonAction::Press))
            std::cout << "p down" << std::endl;
        else if(event.keystroke.is(ButtonAction::Release))
            std::cout << "p up" << std::endl;
        
    }

    if(event.keystroke.has(KeyModifier::ctl))
        std::cout << "CTRL is down" << std::endl;
        
        
}

void event_window_resize(window::InputEvent & event)
{
    window::WindowResizeEvent& resize_event = event.window_resize;

    ManagerScene::get_root_scene()->set_window_size(resize_event.size_f);
}



}