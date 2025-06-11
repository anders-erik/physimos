#include <iostream>


#include "math/vecmat.hh"

#include "math/transform.hh"


#include "quad_manager.hh"

namespace scene {



QuadS2D* QuadManager::add_quad(QuadS2D& new_quad){
    new_quad.id = ++id_index;
    quads.push_back(new_quad);
    return &quads.back();
}

QuadS2D* QuadManager::new_quad(){
    QuadS2D& new_quad = quads.emplace_back();
    new_quad.set_name("quad_default_name");
    new_quad.id = ++id_index;
    return &new_quad;
}

QuadS2D* QuadManager::get_quad_mut(size_t id)
{
    for(auto& quad : quads)
    {
        if(id == quad.get_id())
            return &quad;
    }
    return nullptr;
}

const std::list<QuadS2D>& QuadManager::get_quads()
{
    return quads;
}

std::list<QuadS2D>& QuadManager::get_quads_mut()
{
    return quads;
}


void QuadManager::set_selected(size_t quad_id)
{
    selected_quad = quad_id;
}

QuadS2D* QuadManager::get_selected()
{
    return get_quad_mut(selected_quad);
}

void QuadManager::clear_selection()
{
    selected_quad = 0;
}


void QuadManager::set_hovered(size_t quad_id)
{
    hovered_quad = quad_id;
}

QuadS2D* QuadManager::get_hovered()
{
    return get_quad_mut(hovered_quad);
}

void QuadManager::clear_hovered()
{
    hovered_quad = 0;
}



}
