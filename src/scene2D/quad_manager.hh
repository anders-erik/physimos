
#pragma once

#include <vector>
#include <list>

#include "math/vecmat.hh"
#include "math/shape.hh"
#include "math/box2D.hh"

#include "quadS2D.hh"

using QuadID = size_t;



struct QuadS2D;

/** Global quad manager */
class ManagerQuadS2D 
{
    size_t id_index = 2; // index of 0 = no quad exists for this id

    // std::vector<QuadS2D> quads;
    std::list<QuadS2D> quads; // list to prevent quad reallocation

    size_t selected_id;
    size_t hovered_id;

public:

    ManagerQuadS2D() = default;

    /** Add transfer ownership and get a stable quad id back. */
    QuadS2D* add_quad(QuadS2D& new_quad);
    size_t push_quad(QuadS2D& new_quad);
    QuadS2D* new_quad();
    QuadS2D* get_quad_mut(size_t id);
    const QuadS2D* get_quad(size_t id);
    const std::list<QuadS2D>& get_quads();
    std::list<QuadS2D>& get_quads_mut();
    /** Returns a list of quads given a list of quad ids.
        `nullptr`-checks are performed before returning. */
    std::vector<QuadS2D*> get_quads_from_ids_mut(std::vector<size_t>& quad_ids);

    void set_selected(size_t quad_id);
    bool is_selected_id(size_t quad_id);
    QuadS2D* get_selected();
    void clear_selection();

    void set_hovered(size_t quad_id);
    bool is_hover_id(size_t quad_id);
    QuadS2D* get_hovered();
    void clear_hovered();
};


