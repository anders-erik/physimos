
#include "bc.hh"

#include "scene2D.hh"

#include "lib/log.hh"


namespace BC {



size_t id_count = 0;

std::vector<scene::Scene2D> scenes;
bool scene_is_locked = false;

// std::map<unsigned int, std::any> scene_map;
// unsigned int scene_id_count = 0;




static OptPtr<scene::Scene2D> inline try_find_scene(Tag tag)
{
    for(scene::Scene2D& scene : scenes)
        if(scene.get_tag_id() == tag.id)
            return {&scene};
    
    return {};
}

Pair<BC::Tag, OptPtr<scene::Scene2D>> new_scene_with_lock(std::string label){

    if(scene_is_locked){
        log_error("BC", "Object Access", "Tried to create new scene during scene lock.");
        return {Tag(), {}};
    }

    scene::Scene2D& new_scene = scenes.emplace_back( f2{100, 100} );
    new_scene.set_tag_id(id_count);

    Tag new_tag = {id_count++, Type::scene, label};

    scene_is_locked = true;
    
    return {new_tag, {&new_scene}};
    
    // scene_map[scene_id_count++] = scene::Scene2D {{0,0}};
    // return Tag{scene_id_count-1, Type::scene, label};
}


OptPtr<scene::Scene2D> borrow_scene(Tag tag){
    // scene::Scene2D& scene = std::any_cast<scene::Scene2D&>(scene_map[tag.id]);

    if(scene_is_locked)
        throw std::runtime_error("BC: Tried to get scene during scene lock.");
    
    scene_is_locked = true;

    return try_find_scene(tag);
}


void return_scene(Tag tag){

    scene_is_locked = false;
}



Pair<BC::Tag, OptPtr<scene::Scene2D>> new_scene(scene::Scene2D & _scene){

    // scene::Scene2D& new_scene = scenes.push_back( _scene );

    scenes.push_back( _scene );

    Tag new_tag = {id_count++, Type::scene};

    OptPtr<scene::Scene2D> new_scene = try_find_scene(new_tag);
    // if(new_scene.is_null())
    //     log_error("BC", "SCENE", "Unable to retrieve new scene.");

    return { new_tag, new_scene};
}

Pair<BC::Tag,OptPtr<scene::Scene2D>> new_scene(f2 framebuffer_size)
{
    scene::Scene2D& new_scene = scenes.emplace_back( f2{100, 100} );

    new_scene.set_tag_id(id_count);

    Tag new_tag = {id_count++, Type::scene};
    
    return {new_tag, {&new_scene}};
}

OptPtr<scene::Scene2D> get_scene(Tag tag)
{
    return try_find_scene(tag);
}



}