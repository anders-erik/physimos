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

struct Shape {

    size_t get_point_count(){ return point_count; }

    void print();

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

private:
    shape_t type;
    size_t point_count;
    std::vector<Point> points;
};


void Shape::print(){

    std::cout << std::endl << "Shape.print() : " << std::endl;
    
    if(type == shape_t::line)
        std::cout << "LINE" << std::endl;
    
    for(Point& point : points){
        std::cout << "    x :" << point.x << ", y : " << point.y << std::endl;
        
    }

    std::cout << std::endl;
}


