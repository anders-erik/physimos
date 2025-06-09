#include <iostream>


#include "math/vecmat.hh"

#include "math/transform.hh"

#include "quadS2D.hh"
#include "quad_manager.hh"

namespace scene {



size_t QuadManager::add_quad(QuadS2D& new_quad){
    new_quad.set_quad_id(++id_index);
    quads.push_back(new_quad);
    return id_index;
}

QuadS2D* QuadManager::new_quad(){
    QuadS2D& new_quad = quads.emplace_back();
    new_quad.set_name("lol");
    new_quad.set_quad_id(++id_index);
    return &new_quad;
}

QuadS2D* QuadManager::get_quad(size_t id)
{
    for(auto& quad : quads)
    {
        if(id == quad.get_quad_id())
            return &quad;
    }
    return nullptr;
}


void QuadManager::set_selected(size_t quad_id)
{
    selected_quad = quad_id;
}

QuadS2D* QuadManager::get_selected()
{
    return get_quad(selected_quad);
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
    return get_quad(hovered_quad);
}

void QuadManager::clear_hovered()
{
    hovered_quad = 0;
}



}
