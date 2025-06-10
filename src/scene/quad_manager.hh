
#pragma once

#include <vector>
#include <list>

#include "math/vecmat.hh"
#include "math/shape.hh"
#include "math/box2D.hh"




namespace scene {


struct QuadS2D;


class QuadManager {

    size_t id_index = 1; // index of 0 = no quad exists for this id

    // std::vector<QuadS2D> quads;
    std::list<QuadS2D> quads; // list to prevent quad reallocation

    size_t selected_quad;
    size_t hovered_quad;

public:

    QuadManager() = default;

    /** Add transfer ownership and get a stable quad id back. */
    QuadS2D* add_quad(QuadS2D& new_quad);
    QuadS2D* new_quad();
    QuadS2D* get_quad_mut(size_t id);
    const std::list<QuadS2D>& get_quads();
    std::list<QuadS2D>& get_quads_mut();

    void set_selected(size_t quad_id);
    QuadS2D* get_selected();
    void clear_selection();

    void set_hovered(size_t quad_id);
    QuadS2D* get_hovered();
    void clear_hovered();
};



}