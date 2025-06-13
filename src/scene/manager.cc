#include <list>

#include "lib/log.hh"

#include "window/auxevent.hh"

#include "scene2D.hh"

#include "quad_manager.hh"

#include "manager.hh"

namespace ManagerScene {


static size_t id_count = 2; // Global index count

static bool is_grabbing_a_scene = false;
static Box2D window_box; // The current window viewport box
static Box2D cursor_scene_box; // the box of cursor scene in window coordinates

static window::CursorPosition cursor_pos; // Copy from most recent cursor move event

// static RendererScene2D renderer_scene;

static struct ScenesState {
    const size_t root_id = 1; // root scene is created during init; is indestructable; id==1;
    size_t window_id = 1; // the scene renedered to the main framebuffer
    size_t cursor_id = 1; // scene curently hovered by, or is grabbing, cursor - recieves mouse click and movement events

    /** The cursor is not currently targeting a subscene */
    bool window_scene_is_target(){
        return (cursor_id == window_id) ? true : false;
    }

    /** Reset targeted scene to default: Window. */
    void set_window_as_cursor_scene(){
        cursor_id = window_id;
    }

    /** Reset targeted scene to default: Window. */
    void set_cursor_scene(size_t scene_id){
        cursor_id = scene_id;
    }

} scenes_state;

// std::vector<scene::Scene2D> scenes;
static std::list<scene::Scene2D> scenes;
static QuadManager quad_manager;


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

    window_box.pos = {0.0f, 0.0f}; // always at [0,0]
    window_box.size = _window_size;

    
    scene::Scene2D& root_scene = scenes.emplace_back( window_box.size );

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

void render_all_scene_framebuffers()
{

}


scene::QuadManager& 
get_quad_manager()
{
    return quad_manager;
}



const scene::Scene2D& 
get_root_scene()
{
    scene::Scene2D* root_scene = search_scene_storage(scenes_state.root_id);

    // debug : if root scene, which should NEVER be null, is in fact null
    if(root_scene == nullptr)
			throw std::runtime_error("Root scene is nullptr");

    return *root_scene;
}


scene::Scene2D& 
get_root_scene_mut()
{
    scene::Scene2D* root_scene = search_scene_storage(scenes_state.root_id);

    // debug : if root scene, which should NEVER be null, is in fact null
    if(root_scene == nullptr)
			throw std::runtime_error("Root scene is nullptr");

    return *root_scene;
}


const scene::Scene2D & 
get_window_scene()
{
    scene::Scene2D* window_scene = search_scene_storage(scenes_state.window_id);

    // debug : window scene should NEVER be null
    if(window_scene == nullptr)
			throw std::runtime_error("Window scene is nullptr [immutable]");

    return *window_scene;
}


scene::Scene2D & 
get_window_scene_mut()
{
    scene::Scene2D* window_scene = search_scene_storage(scenes_state.window_id);

    // debug : window scene should NEVER be null
    if(window_scene == nullptr)
			throw std::runtime_error("Window scene is nullptr [mutable]");

    return *window_scene;
}



scene::Scene2D& 
get_cursor_scene_mut()
{   
    // ofter window scene is cursor scene, so we do the most common outcome
    if(scenes_state.window_scene_is_target())
    {
        return get_window_scene_mut();
    }
    
    // find scene
    scene::Scene2D* cursor_scene = search_scene_storage(scenes_state.cursor_id);
    
    // As the cursor scene was not found, we default to window scene as the cursor scene
    if(cursor_scene == nullptr)
    {
        scenes_state.set_window_as_cursor_scene();
        return get_window_scene_mut();
    }

    return *cursor_scene;
}




scene::Scene2D* 
search_scene_storage(size_t id)
{
    for(scene::Scene2D& scene : scenes)
        if(scene.get_id() == id)
            return &scene;

    return nullptr;
}



void 
requery_cursor_target(window::CursorPosition& _cursor_pos)
{
    auto& window_scene = get_window_scene_mut();
    
    window_scene.try_hover_quad(); // ALWAYS highlight hovered quad in window scene


    QuadQuery quad_query = window_scene.try_find_quad_in_viewbox(_cursor_pos.normalized); 

    if(quad_query.quad_id != 0)
    {
        auto* quad_p = ManagerScene::get_quad_manager().get_quad_mut(quad_query.quad_id);
        if(quad_p == nullptr) return;
        auto& quad = *quad_p;

        if(!quad.is_scene2D()) 
            return;

        // Set up subscene as cursor scene
        scenes_state.set_cursor_scene(quad.get_object_id());

        cursor_scene_box = Box2D::from_normalized_box(
            window_box,
            quad_query.normalized_viewbox_coords
        );

        auto& cursor_scene = get_cursor_scene_mut();
        // Viewboxes ONLY accepts normalized coordinate
        f2 cursor_in_normalized_viewbox = cursor_scene_box.to_normalized(_cursor_pos.sane);
        cursor_scene.set_cursor_pos(cursor_in_normalized_viewbox);
        cursor_scene.try_hover_quad();
    }
    else 
    {
        window_scene.set_cursor_pos(_cursor_pos.normalized);
        scenes_state.set_window_as_cursor_scene();
        cursor_scene_box = window_box;
    }

}


bool is_grabbing_cursor()
{
    return is_grabbing_a_scene;
}


void clear_cursor_hovers()
{
    auto& q_manager = ManagerScene::get_quad_manager();
    Scene2D& cursor_scene = get_cursor_scene_mut();
    q_manager.clear_hovered();
    scenes_state.set_window_as_cursor_scene();

    // make sure window, which is now cursor scene, is also clear
    q_manager.clear_hovered();
}








void event_scroll(window::InputEvent& event)
{
    scene::Scene2D& current_target = ManagerScene::get_cursor_scene_mut();
    auto event_result = current_target.handle_scroll(event);
    if(event_result.is_grab())
        is_grabbing_a_scene = true;
    else
        is_grabbing_a_scene = false;
}


void 
event_move(window::InputEvent& event)
{
    auto& move_event = event.mouse_movement;
    cursor_pos = move_event.cursor_new;
	auto& cursor_prev = move_event.cursor_prev;

    
    Scene2D& cursor_scene = ManagerScene::get_cursor_scene_mut();

    scene::PointerMovement2D scene_pointer_move;
    scene_pointer_move.pos_norm_prev = cursor_scene_box.to_normalized(cursor_prev.sane);
    scene_pointer_move.pos_norm_curr = cursor_scene_box.to_normalized(cursor_pos.sane);
    scene_pointer_move.event = event;

    auto event_result =  cursor_scene.handle_pointer_move(scene_pointer_move);
    if(event_result.is_grab())
        is_grabbing_a_scene = true;
    else
        is_grabbing_a_scene = false;
}


void 
event_mouse_button(window::InputEvent& event)
{
	auto& mouse_button_event = event.mouse_button;
    
    Scene2D& cursor_scene = ManagerScene::get_cursor_scene_mut();

    auto event_result = cursor_scene.handle_pointer_click(event);
    if(event_result.is_grab())
        is_grabbing_a_scene = true;
    else
        is_grabbing_a_scene = false;


    // select the subscene quad in window scene
    if(mouse_button_event.is_left_down())
    {
        bool success = cursor_scene.try_select_quad();
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

void 
events_user_all(window::InputEvent & event)
{
    using namespace window;

    // std::cout << "root_id  : " << scenes_state.root_id << std::endl;
    // std::cout << "window_id: " << scenes_state.window_id << std::endl;
    // std::cout << "cursor_id: " << scenes_state.cursor_id << std::endl;
    // cursor_scene_window_box.print();
    // cursor_scene_box.print();
    // std::cout << "is_grabbing_a_scene: " << is_grabbing_a_scene << std::endl;

    switch (event.type)
    {

    case EventType::MouseMove :
        event_move(event);
        break;

    case EventType::MouseButton :
        event_mouse_button(event);
        break;

    case EventType::MouseScroll :
        event_scroll(event);
        break;

    case EventType::Keystroke :
        event_keystroke(event);
        break;
    
    default:
        println("WARN: unknown event handled in scene managers primary event handler.");
        break;
    }
}




void event_window_resize(window::InputEvent& event)
{
    window::WindowResizeEvent& resize_event = event.window_resize;

    window_box.size = resize_event.size_f;
    
    // TODO: update the window scene?
    ManagerScene::get_root_scene_mut().set_framebuffer_size(resize_event.size_f);
}



}