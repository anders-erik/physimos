
#pragma once

#include "math/vecmat.hh"

/** AABB in 2D */
struct Box2D {
    f2 pos;
    f2 size;


    /** Returns a congruent subbox using the two passed. */
    Box2D subbox_to_normalized(Box2D parent_box, Box2D child_box);

    /** Returns the passed box point in normalized box coordinates. */
    f2 to_normalized(f2 point);
    /** Returns the passed normalized point in box coordinates. */
    f2 to_box(f2 normalized);
    f2 to_box_delta(f2 normalized);

    bool contains_point(f2 p);

    m3f3 get_matrix_m3f3();

    void print();

    /** Converts a subbox into normalized box coordinates. */
    static Box2D to_normalized_box(const Box2D& box, const Box2D& subbox);
    /** Converts a normalized subbox into box coordinates. */
    static Box2D from_normalized_box(const Box2D & box, const Box2D & n_subbox);
};
