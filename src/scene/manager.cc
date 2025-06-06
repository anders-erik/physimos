#include <list>

#include "manager.hh"

#include "scene2D.hh"

#include "lib/log.hh"


namespace ManagerScene {


static size_t id_count = 2; // Global index count

static struct SceneState {
    const size_t root_id = 1; // root scene is created during init - always id==1 
    size_t window_id = 1; // the scene renedered to the main framebuffer
    size_t active_id = 1; // Currently selected scene - recieves input events
    size_t cursor_target_id = 1; // scene curently capturing the pointer
} state;

// std::vector<scene::Scene2D> scenes;
static std::list<scene::Scene2D> scenes;


/** 
    Returns a unique, not equal to 0 or 1, unsigned integer.
    An id of 0 is globally understood as having no object asssociated with it; ids of 1 is always the fixed root scene.
 */
size_t new_unique_id()
{
    id_count++;
    if(id_count == 0)
        id_count += 2;
    if(id_count == 1)
        id_count++;
    
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

scene::Scene2D* init(f2 window_size)
{
    if(scenes.size() > 1)
    {
        throw std::runtime_error("Can't init the scene manager twice");
    }
    
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

    scene::Scene2D* current_scene_target = get_root_scene()->try_find_target_scene(cursor_pos_window_normalized);

    state.cursor_target_id = current_scene_target->get_id();;
}

void clear_current_target()
{
    // clear all scenes highlighting
    for(scene::Scene2D& scene : scenes)
        scene.clear_hovers();

    scene::Scene2D* cursor_target_scene = get_scene(state.cursor_target_id);
    
    state.cursor_target_id = 0;
}

scene::Scene2D * get_current_target()
{
    return get_scene(state.cursor_target_id);
}



}