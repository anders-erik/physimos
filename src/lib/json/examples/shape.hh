#include <string>
#include <vector>
#include <cmath>

#include "math/vecmat.hh"

#include "../physon.hh"
#include "../physon_types.hh"



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

    void print();
    shape_t get_type();
    size_t get_point_count();
    std::vector<f2>& get_points();
    f2& operator[](size_t index) {return points[index];};

    // creates the regular polygon shapes/dihedral group
    static Shape create(shape_t type);
    static std::string get_shape_name(shape_t type);

    Shape (f2 p1, f2 p2) {
        points.push_back(p1);
        points.push_back(p2);
        point_count = 2;
        type = shape_t::line;
    };
    Shape (const std::vector<f2>& points) {
        point_count = points.size();

        if(point_count == 0)
            throw std::runtime_error("Constructing shape with empty point array.");

        this->points = points;

        switch (point_count){
            case 1: type = shape_t::point;      break;
            case 2: type = shape_t::line;       break;
            case 3: type = shape_t::triangle;   break;
            case 4: type = shape_t::quad;       break;
            default:type = shape_t::polygon;    break;
        }
        
    };

};

Shape Shape::create(shape_t type){
    std::vector<f2> points;
    float point_count; 

    switch (type) {
    
    case shape_t::point:    point_count = 1.0f; break;
    case shape_t::line:     point_count = 2.0f; break;
    case shape_t::triangle: point_count = 3.0f; break;
    case shape_t::quad:     point_count = 4.0f; break;

    case shape_t::polygon:  point_count = 8.0f; break;
    
    }

    float twoPI = 6.28f;
    float angle_step_size = twoPI / point_count;

    for(float th = 0.0f; th < twoPI; th += angle_step_size )
        points.emplace_back( f2{cosf(th), sinf(th)} );

    return Shape{points};
}

shape_t Shape::get_type(){
    return type;
}

std::vector<f2>& Shape::get_points(){
    return points;
}
size_t Shape::get_point_count(){ 
    return point_count; 
}

std::string Shape::get_shape_name(shape_t type){
    std::string shape_name;

    switch (type)
    {

    case shape_t::point:
        shape_name = "point";
        break;
    case shape_t::line:
        shape_name = "line";
        break;
    case shape_t::triangle:
        shape_name = "triangle";
        break;
    case shape_t::quad:
        shape_name = "quad";
        break;
    case shape_t::polygon:
        shape_name = "polygon";
        break;
    
    default:
        shape_name = "unknown";
        break;
    }

    return shape_name;
}

void Shape::print(){

    std::cout <<"\nShape.print()\n\n";
    
    std::cout << get_shape_name(type) << std::endl;
    
    for(f2& point : points){
        std::cout << "    x :" << point.x << ", y : " << point.y << std::endl;
        
    }

    std::cout << std::endl;
}


