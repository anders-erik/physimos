#include <iostream>
#include <list>

#include "lib/log.hh"

#include "window/auxevent.hh"

#include "scene2D.hh"
#include "scene.hh"

#include "quad_manager.hh"

#include "manager.hh"


namespace ManagerScene 
{


static InputStateSceneManager input_state;

static SceneID id_count = 2; // Global index count

static bool is_grabbing_a_scene = false;
static Box2D window_box; // The current window viewport box
static Box2D quad_box; // the box of cursor scene in window coordinates

static window::CursorPosition cursor_pos; // Copy from most recent cursor move event

static SceneBase* window_scene = nullptr; 

// static RendererScene2D renderer_scene;

static struct ScenesState
{
    const SceneID root_id = 1; // root scene is created during init; is indestructable; id==1;
    SceneID window_id = 1; // the scene renedered to the main framebuffer
    SceneID cursor_id = 1; // scene curently hovered by, or is grabbing, cursor - recieves mouse click and movement events

    /** The cursor is not currently targeting a subscene */
    bool window_scene_is_target(){
        return (cursor_id == window_id) ? true : false;
    }

    /** Reset targeted scene to default: Window. */
    void set_window_as_cursor_scene(){
        cursor_id = window_id;
    }

    /** Reset targeted scene to default: Window. */
    void set_cursor_scene(SceneID scene_id){
        cursor_id = scene_id;
    }

} scenes_state;

// std::vector<scene::Scene2D> scenes;
static std::list<scene::Scene2D> scenes2D;
static std::list<Scene3D> scenes3D;
static QuadManager quad_manager;


/** 
    Returns a unique, not equal to 0 or 1, unsigned integer.
    An id of 0 is globally understood as having no object asssociated with it; ids of 1 is always the fixed root scene.
 */
SceneID new_unique_id()
{
    ++id_count;

    if (id_count == 0)
        id_count += 2;
    else if (id_count == 1)
        id_count += 1;
    
    return id_count;
}



Scene3D& init(f2 _window_size)
{
    if(scenes2D.size() > 1)
    {
        throw std::runtime_error("Can't init the scene manager twice");
    }

    window_box.pos = {0.0f, 0.0f}; // always at [0,0]
    window_box.size = _window_size;


    Scene3D& root_scene = scenes3D.emplace_back();

    root_scene.scene_id = scenes_state.root_id;
    root_scene.type = SceneType::D3;

    window_scene = &root_scene;

    scenes_state.window_id = scenes_state.root_id;
    scenes_state.cursor_id = 0;

    return root_scene;
}

Scene3D& 
get_root_scene_3D_mut()
{
    return scenes3D.front();
}


SceneID push_scene2D(scene::Scene2D& _scene){
    
    SceneID new_id = new_unique_id();

    _scene.scene_id = new_id;
    _scene.type = SceneType::D2;


    _scene.set_id(new_id);

    scenes2D.push_back( _scene );

    return new_id;
}



scene::QuadManager& 
get_quad_manager()
{
    return quad_manager;
}



const Scene3D& 
get_root_scene()
{
    return scenes3D.front();
}


Scene3D& 
get_root_scene_mut()
{
    return scenes3D.front();
}


const SceneBase& 
get_window_scene()
{
    return *window_scene;
}


SceneBase& 
get_window_scene_mut()
{
    return *window_scene;
}




SceneBase*
search_scene_storage(SceneID id)
{
    for(scene::Scene2D& scene : scenes2D)
        if(scene.scene_id == id)
            return &scene;

    return nullptr;
}




bool is_grabbing_cursor()
{
    return is_grabbing_a_scene;
}


void clear_cursor_hovers()
{
    auto& q_manager = ManagerScene::get_quad_manager();
    q_manager.clear_hovered();
}








void event_scroll(window::InputEvent& event)
{
    switch (window_scene->type)
    {

        case SceneType::D2 :
            {
            auto event_result = ((scene::Scene2D*) window_scene)->handle_scroll(event);
            if(event_result.grabbed_mouse())
                is_grabbing_a_scene = true;
            else
                is_grabbing_a_scene = false;
            }
            break;
        
        case SceneType::D3 :
            std::cout << "D3 scroll" << std::endl;
            break;
    
        // default:
        //     break;
    }
    
}


void 
event_move(window::InputEvent& event)
{

    switch (window_scene->type)
    {
    case SceneType::D2 :
        {
        auto event_result = ((scene::Scene2D*) window_scene)->handle_scroll(event);
        if(event_result.grabbed_mouse())
            is_grabbing_a_scene = true;
        else
            is_grabbing_a_scene = false;
        }
        break;

    case D3:
        std::cout << "D3 mouse move" << std::endl;
        break;
    
    // default:
    //     break;
    }
}


void 
event_mouse_button(window::InputEvent& event)
{
	auto& mouse_button_event = event.mouse_button;

    switch (window_scene->type)
    {

    case SceneType::D2 :
        {
        auto event_result = ((scene::Scene2D*) window_scene)->handle_pointer_click(event);
        if(event_result.grabbed_mouse())
            is_grabbing_a_scene = true;
        else
            is_grabbing_a_scene = false;
        }
        break;
    

    case SceneType::D3:
        std::cout << "D3 mouse button" << std::endl;
        
        break;

    }


    // select the subscene quad in window scene
    if(mouse_button_event.is_left_down())
    {
        // bool success = cursor_scene.try_select_quad();
    }

}

void event_keystroke(window::InputEvent& event)
{
    using namespace window;

    
    
    switch (window_scene->type)
    {
    case SceneType::D2 :
        {
        auto event_result = ((scene::Scene2D*) window_scene)->handle_pointer_click(event);
        if(event_result.grabbed_mouse())
            is_grabbing_a_scene = true;
        else
            is_grabbing_a_scene = false;
        }
        break;

    // case SceneType::D3 :

    //     if(event.keystroke.is(Key::p)){

    //         if(event.keystroke.is(ButtonAction::Press))
    //             std::cout << "p down" << std::endl;
    //         else if(event.keystroke.is(ButtonAction::Release))
    //             std::cout << "p up" << std::endl;
            
    //     }

    //     if(event.keystroke.has(KeyModifierState::ctl))
    //         std::cout << "CTRL is down" << std::endl;

    //     break;
    
    // default:
    //     break;
    }
        
}



InputResponse 
events_user_all(window::InputEvent& event)
{
    using namespace window;

    // std::cout << "root_id  : " << scenes_state.root_id << std::endl;
    // std::cout << "window_id: " << scenes_state.window_id << std::endl;
    // std::cout << "cursor_id: " << scenes_state.cursor_id << std::endl;
    // cursor_scene_window_box.print();
    // cursor_scene_box.print();
    // std::cout << "is_grabbing_a_scene: " << is_grabbing_a_scene << std::endl;



    // auto& window_scene = get_window_scene_mut();
    // window_scene.try_hover_quad(); // ALWAYS try to highlight hovered quad in window scene

    if(window_scene->is_3d())
    {
        ((Scene3D*)window_scene)->handle_input(event);
        return {};
    }

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

    return {};
}




void event_window_resize(window::WindowResizeEvent& window_resize)
{

    window_box.size = window_resize.size_f;
    
    // TODO: update the window scene?
    // ManagerScene::get_root_scene_mut().set_framebuffer_size(window_resize.size_f);
}



}