#include <list>

#include "manager.hh"

#include "scene2D.hh"

#include "lib/log.hh"


namespace ManagerScene {


static size_t id_count = 1; // Global index count

// std::vector<scene::Scene2D> scenes;
static std::list<scene::Scene2D> scenes;


/** 
    Returns a unique, non-zero, unsigned integer.
    An id of 0 is globally understood as having no object asssociated with it.
 */
size_t new_unique_id()
{
    id_count++;
    if(id_count == 0)
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

scene::Scene2D* get_scene(size_t id)
{
    return try_find_scene(id);
}



}