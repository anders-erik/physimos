
#pragma once


#include <vector>

#include "lib/str.hh"

#include "window/auxevent.hh"

#include "scene/quad.hh"

struct Manager2D;

struct QuadState 
{
    TagO capturing_tag;

    bool is_capturing() { return capturing_tag.not_null(); }
};

class ManagerQuad
{
    QID qid_index = 1;

public:

    std::vector<QuadO>  quados;

    QuadState state;

    OID new_qid();

    Quad* find_quad_qid(QID qid);
    Quad* find_quad_oid(OID oid);

    bool try_new_quad_grab(window::InputEvent& event, TagO sampled_tag);
    bool try_release_quad(window::InputEvent& event, TagO sampled_tag);

    InputResponse handle_event(window::InputEvent& event, Manager2D& manager_2D);

};




