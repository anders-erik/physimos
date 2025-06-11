#include <list>

#include "lib/log.hh"

#include "window/auxevent.hh"

#include "scene2D.hh"

#include "manager.hh"

namespace ManagerScene {


static size_t id_count = 2; // Global index count

static bool is_grabbing_a_scene = false;
static f2 window_size = {0.0f, 0.0f};
static Box2D target_window_box;

static window::CursorPosition cursor_pos; // Copy from most recent cursor move event

static struct ScenesState {
    const size_t root_id = 1; // root scene is created during init; is indestructable; id==1;
    size_t window_id = 1; // the scene renedered to the main framebuffer
    size_t cursor_id = 1; // scene curently hovered by, or is grabbing, cursor - recieves mouse click and movement events
} scenes_state;

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



scene::Scene2D* init(f2 _window_size)
{
    if(scenes.size() > 1)
    {
        throw std::runtime_error("Can't init the scene manager twice");
    }

    window_size = _window_size;
    
    scene::Scene2D& root_scene = scenes.emplace_back( window_size );

    root_scene.set_id(scenes_state.root_id);

    scenes_state.window_id = scenes_state.root_id;
    scenes_state.cursor_id = 0;

    return &root_scene;
}


size_t push_scene(scene::Scene2D& _scene){
    
    size_t new_id = new_unique_id();

    _scene.set_id(new_id);

    scenes.push_back( _scene );

    return new_id;
}



scene::Scene2D& get_root_scene_mut()
{
    scene::Scene2D* root_scene = try_find_scene(scenes_state.root_id);

    // debug : if root scene, which should NEVER be null, is in fact null
    if(root_scene == nullptr)
			throw std::runtime_error("Root scene is nullptr");

    return *root_scene;
}

const scene::Scene2D& get_root_scene()
{
    scene::Scene2D* root_scene = try_find_scene(scenes_state.root_id);

    // debug : if root scene, which should NEVER be null, is in fact null
    if(root_scene == nullptr)
			throw std::runtime_error("Root scene is nullptr");

    return *root_scene;
}


scene::Scene2D* try_find_scene(size_t id)
{
    for(scene::Scene2D& scene : scenes)
        if(scene.get_id() == id)
            return &scene;

    return nullptr;
}




void set_cursor_pos_bypass_grab(window::CursorPosition& _cursor_pos)
{
    // cursor_pos = _cursor_pos;

    // maintain most recent target
    // if(is_grabbing_a_scene)
    //     return;

    f2 window_pos = {0.0f, 0.0f}; // window scene is always filling the whole window
    Box2D window_box = {
        window_pos,
        window_size,
    };
    
    // TODO: Root scene is currently always the window scene
    auto* window_scene = &get_root_scene_mut();

    window_scene->set_cursor_pos(cursor_pos.normalized);
    window_scene->set_window_box(window_box); // This is superfluous -- size should already match window!
    window_scene->try_hover_quad();

    auto* current_scene_target = window_scene->try_find_window_subscene();

    if(current_scene_target == nullptr)
        throw std::runtime_error("Try find current scene target was nullptr.");

    

    scenes_state.cursor_id = current_scene_target->get_id();
    current_scene_target->try_hover_quad();
    target_window_box = current_scene_target->get_window_box();
}

bool is_grabbing_cursor()
{
    return is_grabbing_a_scene;
}

void clear_cursor_hovers()
{
    // TODO: Implement this for a window scene that might not be the root scene

    auto& root_scene = get_root_scene_mut();
    
    // root_scene.clear_quad_hovers();
    root_scene.quad_manager.clear_hovered();
    // root_scene.quad_manager.clear_selection();
    // root_scene.clear_cursor_grab();

    for(auto& quad : root_scene.quad_manager.get_quads_mut())
    {
        if(quad.is_scene2D())
        {
            auto* scene = ManagerScene::try_find_scene(quad.get_object_id());
            if(scene != nullptr)
            {
                scene->quad_manager.clear_hovered();
                // scene->clear_cursor_grab();
                // scene->quad_manager.clear_selection();
            }
        }
        
    }

    scenes_state.cursor_id = 1;
}

void clear_cursor_selections()
{

}

scene::Scene2D * get_current_target()
{
    return try_find_scene(scenes_state.cursor_id);
}







void event_scroll(window::InputEvent& event)
{
    scene::Scene2D* current_target = ManagerScene::get_current_target();
    if(current_target != nullptr)
    {
        current_target->handle_scroll(event);
    }
}


void 
event_move(window::InputEvent& event)
{
    cursor_pos = event.mouse_movement.cursor_new;
	window::CursorPosition& cursor_prev = event.mouse_movement.cursor_prev;

    scene::Scene2D* current_target = ManagerScene::get_current_target();
		if(current_target == nullptr)
			return;
    
    // box of current target in window coordinates
    Box2D _target_window_box = current_target->get_window_box();

    scene::PointerMovement2D scene_pointer_move;
    scene_pointer_move.pos_norm_prev = _target_window_box.to_normalized(cursor_prev.sane);
    scene_pointer_move.pos_norm_curr = _target_window_box.to_normalized(cursor_pos.sane);
    scene_pointer_move.event = event;

    current_target->handle_pointer_move(scene_pointer_move);
}


void 
event_mouse_button(window::InputEvent& event)
{
	window::MouseButtonEvent mouse_button_event = event.mouse_button;
    
    scene::Scene2D* current_target = ManagerScene::get_current_target();
        if(current_target == nullptr)
            return;
	
	scene::PointerClick2D pointer_click = {
        current_target->get_window_box().to_normalized(cursor_pos.sane),
		event
	};

    current_target->handle_pointer_click(pointer_click);


    // select the subscene quad in window scene
    if(mouse_button_event.is_left_down())
    {
        auto* window_scene = ManagerScene::try_find_scene(scenes_state.window_id);
        window_scene->try_select_quad();
    }


    if(mouse_button_event.is_middle_down())
    {
        is_grabbing_a_scene = true;
    }
    else if(mouse_button_event.is_middle_up())
    {
        is_grabbing_a_scene = false;
        clear_cursor_hovers();
    }

}

void event_keystroke(window::InputEvent& event)
{
    using namespace window;

    if(event.keystroke.is(Key::p)){

        if(event.keystroke.is(ButtonAction::Press))
            std::cout << "p down" << std::endl;
        else if(event.keystroke.is(ButtonAction::Release))
            std::cout << "p up" << std::endl;
        
    }

    if(event.keystroke.has(KeyModifierState::ctl))
        std::cout << "CTRL is down" << std::endl;
        
        
}

void event_window_resize(window::InputEvent& event)
{
    window::WindowResizeEvent& resize_event = event.window_resize;

    window_size = resize_event.size_f;
    
    // TODO: update the window scene?
    ManagerScene::get_root_scene_mut().set_framebuffer_size(resize_event.size_f);
}



}