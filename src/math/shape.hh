#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <stdexcept>


#include "math/vecmat.hh"
#include "math/box2D.hh"

// Keeps track of min/max values of x and y
class BoundingRect2D {

public:
    Box2D box;

    f2 min = { MAXFLOAT,  MAXFLOAT};
    f2 max = {-MAXFLOAT, -MAXFLOAT};

    void update(f2 point); // Updates min/max by comparing to provided point
    void move(f2 delta_pos); // min/max & box according to delta position

    void reset();
};

enum class shape_t {
    point,
    line,
    triangle,
    quad,
    polygon,
};


class Shape {

    shape_t type;
    size_t point_count;
    std::vector<f2> points;
    BoundingRect2D bounding_rect;

public:
    Shape (f2 p1, f2 p2);
    Shape (const std::vector<f2>& points);

    f2& operator[](size_t index) {return points[index];};

    bool is(shape_t type);
    shape_t get_type();
    size_t get_point_count();
    std::vector<f2>& get_points();

    void set_bounds(BoundingRect2D new_bounding_rect);
    void move(f2 move_vec);
    void rotate(float theta);
    // creates the regular polygon shapes/dihedralgroup
    static Shape create(shape_t type);
    static std::string get_shape_name(shape_t type); 

    void print();

};


