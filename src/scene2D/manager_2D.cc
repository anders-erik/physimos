
#include "rend/rend_manager.hh"

#include "scene2D/scene2D.hh"

#include "manager_2D.hh"




SID Manager2D::new_sid_2D()
{
    ++sid_2D_counter;

    if (sid_2D_counter == 0)
        throw std::runtime_error("SID 2D counter was equal to 0.");
    if( sid_2D_counter > SID_MAX_BASE)
        throw std::runtime_error("SID 2D counter exceeded SID_MAX_BASE.");

    return sid_2D_counter | SID_2D;
}




SID Manager2D::push_scene2D(Scene2D& new_scene)
{
    SID sid = new_sid_2D();


    // Create framebuffer
    auto& renderer_2D = Rend::Manager::get_renderer_scene2D();
    f2 fb_size_s = new_scene.get_framebuffer_size();
    ui2 framebuffer_size = {fb_size_s.x, fb_size_s.y};
    renderer_2D.create_scene_framebuffer(sid, framebuffer_size);


    new_scene.sid = sid;
    new_scene.set_id(sid);

    scenes2D.push_back( new_scene );

    return sid;
}



Scene2D* Manager2D::
search_scene_storage_2D(SID id)
{
    for(Scene2D& scene : scenes2D)
        if(scene.sid == id)
            return &scene;

    return nullptr;
}


std::list<Scene2D>& Manager2D::
get_all_scene2D()
{
    return scenes2D;
}
