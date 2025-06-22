#include <iostream>
#include <list>

#include "lib/log.hh"

#include "window/auxevent.hh"

#include "rend/rend_manager.hh"

#include "scene.hh"

#include "scene2D/scene2D.hh"
#include "scene2D/manager_2D.hh"
#include "scene2D/quad_manager.hh"

#include "manager.hh"


namespace ManagerScene 
{

static bool init_flag = false;

static Manager2D manager_2D;

static SID sid_3D_counter = 2; // Global index count

static f2 window_size; // The current window viewport box

static Scene3D root_scene;
static Scene3D* window_scene = nullptr;

static std::list<Scene3D> scenes3D; // list of non-root 3D scenes


static std::list<Scene2D> scenes2D;
static ManagerQuadS2D quad_manager;



/** 
    Returns a unique, not equal to 0 or 1, unsigned integer.
    An id of 0 is globally understood as having no object asssociated with it; ids of 1 is always the fixed root scene.
 */
SID new_sid_3D()
{
    ++sid_3D_counter;

    if (sid_3D_counter == 0)
        throw std::runtime_error("SID 3d counter was equal to 0.");
    else if (sid_3D_counter == 1)
        throw std::runtime_error("SID 3d counter was equal to 1.");
    
    return sid_3D_counter | SID_3D;
}



Scene3D& init(f2 _window_size)
{
    if(init_flag)
        throw std::runtime_error("Can't init the scene manager twice");
    init_flag = true;


    window_size = _window_size;

    root_scene.sid = 1 | SID_3D;

    window_scene = &root_scene;

    return root_scene;
}



ManagerQuadS2D& 
get_quad_manager()
{
    return quad_manager;
}



const Scene3D& 
get_root_scene()
{
    return root_scene;
}


Scene3D& 
get_root_scene_mut()
{
    return root_scene;
}


const Scene3D& 
get_window_scene()
{
    return *window_scene;
}


Scene3D& 
get_window_scene_mut()
{
    return *window_scene;
}




InputResponse event_send_window_scene(window::InputEvent& event)
{
    window_scene->handle_input(event);
    return InputResponse();
}




void event_window_resize(window::WindowResizeEvent& window_resize)
{
    window_size = window_resize.size_f;
    window_scene->handle_window_resize(window_resize);
}




Manager2D& get_manager_2D()
{
    return manager_2D;
}


}