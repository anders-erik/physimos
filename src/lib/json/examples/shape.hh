#include <string>
#include <vector>

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
    std::vector<Point> points;

public:

    void print();
    std::string get_shape_name();
    size_t get_point_count(){ return point_count; }

    Shape (Point p1, Point p2) {
        points.push_back(p1);
        points.push_back(p2);
        point_count = 2;
        type = shape_t::line;
    };
    Shape (const std::vector<Point>& points) {
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

std::string Shape::get_shape_name(){
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
    
    std::cout << get_shape_name() << std::endl;
    
    for(Point& point : points){
        std::cout << "    x :" << point.x << ", y : " << point.y << std::endl;
        
    }

    std::cout << std::endl;
}


