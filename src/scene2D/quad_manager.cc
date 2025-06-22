#include <iostream>


#include "math/vecmat.hh"

#include "math/transform.hh"


#include "quad_manager.hh"




QuadS2D* ManagerQuadS2D::add_quad(QuadS2D& new_quad){
    new_quad.id = ++id_index;
    quads.push_back(new_quad);
    return &quads.back();
}

size_t ManagerQuadS2D::push_quad(QuadS2D& new_quad){
    new_quad.id = ++id_index;
    quads.push_back(new_quad);
    return new_quad.id;
}

QuadS2D* ManagerQuadS2D::new_quad(){
    QuadS2D& new_quad = quads.emplace_back();
    new_quad.set_name("quad_default_name");
    new_quad.id = ++id_index;
    return &new_quad;
}

QuadS2D* ManagerQuadS2D::get_quad_mut(size_t id)
{
    for(auto& quad : quads)
    {
        if(id == quad.get_id())
            return &quad;
    }
    return nullptr;
}

const QuadS2D* ManagerQuadS2D::get_quad(size_t id)
{
    for(auto& quad : quads)
    {
        if(id == quad.get_id())
            return &quad;
    }
    return nullptr;
}

const std::list<QuadS2D>& ManagerQuadS2D::get_quads()
{
    return quads;
}

std::list<QuadS2D>& ManagerQuadS2D::get_quads_mut()
{
    return quads;
}

std::vector<QuadS2D*> ManagerQuadS2D::get_quads_from_ids_mut(std::vector<size_t>& quad_ids)
{
    std::vector<QuadS2D*> matched_quads;
    
    for(size_t quad_id : quad_ids)
    {
        auto* quad = get_quad_mut(quad_id);
        if(quad != nullptr)
            matched_quads.push_back(quad);
    }

    return matched_quads;
}


void ManagerQuadS2D::set_selected(size_t quad_id)
{
    selected_id = quad_id;
}

bool ManagerQuadS2D::is_selected_id(size_t quad_id)
{
    return selected_id == quad_id ? true : false;
}

QuadS2D* ManagerQuadS2D::get_selected()
{
    return get_quad_mut(selected_id);
}

void ManagerQuadS2D::clear_selection()
{
    selected_id = 0;
}


void ManagerQuadS2D::set_hovered(size_t quad_id)
{
    hovered_id = quad_id;
}

bool ManagerQuadS2D::is_hover_id(size_t quad_id)
{
    return hovered_id == quad_id ? true : false;
}

QuadS2D* ManagerQuadS2D::get_hovered()
{
    return get_quad_mut(hovered_id);
}

void ManagerQuadS2D::clear_hovered()
{
    hovered_id = 0;
}



