
#pragma once

#include "math/vecmat.hh"

/** AABB in 2D */
struct Box2D {
    f2 pos;
    f2 size;


    /** Returns a sub-box of proportial dimensions as the two passed. */
    Box2D new_congruent_subbox(Box2D parent_box, Box2D child_box);

    bool contains_point(f2 p);

    void print();
};
