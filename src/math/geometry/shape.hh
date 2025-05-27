#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>


#include "math/vecmat.hh"



enum class shape_t {
    point,
    line,
    triangle,
    quad,
    polygon,
};

struct Point { double x; double y; };

class Shape {

    shape_t type;
    size_t point_count;
    std::vector<f2> points;

public:
    Shape (f2 p1, f2 p2);
    Shape (const std::vector<f2>& points);

    f2& operator[](size_t index) {return points[index];};

    bool is(shape_t type);
    shape_t get_type();
    size_t get_point_count();
    std::vector<f2>& get_points();

    void move(f2 move_vec);
    // creates the regular polygon shapes/dihedralgroup
    static Shape create(shape_t type);
    static std::string get_shape_name(shape_t type); 

    void print();

};


