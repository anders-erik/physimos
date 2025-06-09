
#pragma once

#include "math/vecmat.hh"

/** AABB in 2D */
struct Box2D {
    f2 pos;
    f2 size;


    /** Returns a sub-box of proportial dimensions as the two passed. */
    Box2D new_congruent_subbox(Box2D parent_box, Box2D child_box);
    /** Returns the passed box point in normalized box coordinates. */
    f2 to_normalized(f2 point);
    /** Returns the passed normalized point in box coordinates. */
    f2 to_box(f2 normalized);
    f2 to_box_delta(f2 normalized);

    bool contains_point(f2 p);

    m3f3 get_matrix_m3f3();

    void print();
};
