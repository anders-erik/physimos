
#pragma once

#include "lib/str.hh"

#include "math/vecmat.hh"
#include "math/shape.hh"
#include "math/box2D.hh"




namespace scene {



/** 
    Math Box2D wrapper with a quad id, name, and texture id.  
    Currently implemented as an AABB.
*/
class QuadS2D 
{
    Box2D box;  // Composition instead of inheritence
    m3f3 M_m_s; // Ownership of queried matrix for safe use of references

    size_t id; // index of 0 = no quad exists for this id
    Str name; // given name

    unsigned int texture_id; // To be owned by subscene, canvas, etc.

public:

    QuadS2D() = default;

    void set_id(size_t new_id);
    size_t get_id();

    void set_name(Str new_name);
    Str& get_name();

    void set_box(f2 pos, f2 size);
    Box2D get_box();
    m3f3 get_model_matrix();

    void set_texture_id(unsigned int new_id);
    unsigned int get_texture_id();


    // Compares provided point to Box2D bounds [scene coordinates]
    bool contains_cursor(f2 cursor_pos_scene_coords);
    // Converts provided scene-point to local quad-coordinates ( [0,1]x[0,1] )
    f2 scene_to_quad_normalized(f2 cursor_pos_scene_coords);

};



}