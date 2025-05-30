
#include "shape.hh"



void BoundingRect2D::update(f2 point){
    // x min/max
    if(point.x < min.x)
        min.x = point.x;
    if(point.x > max.x)
        max.x = point.x;
    
    // y min/max
    if(point.y < min.y)
        min.y = point.y;
    if(point.y > max.y)
        max.y = point.y;

    box.pos.x = min.x;
    box.pos.y = min.y;
    box.size.x = max.x - min.x;
    box.size.y = max.y - min.y;
}
void BoundingRect2D::move(f2 delta_pos){
    min += delta_pos;
    max += delta_pos;

    box.pos += delta_pos;

    // max.x += delta_pos.x;
    // min.x += delta_pos.x;

    // max.y += delta_pos.y;
    // min.y += delta_pos.y;
}

void BoundingRect2D::reset(){
    min = { MAXFLOAT,  MAXFLOAT};
    max = {-MAXFLOAT, -MAXFLOAT};
    box.pos = {0.0f, 0.0f};
    box.size = {0.0f, 0.0f};
}


Shape::Shape(f2 p1, f2 p2){
    points.push_back(p1);
    points.push_back(p2);
    point_count = 2;
    type = shape_t::line;
}

Shape::Shape(const std::vector<f2>& points) {
    point_count = points.size();

    if(point_count == 0)
        throw std::runtime_error("Constructing shape with empty point array.");

    this->points = points;

    for(f2& point : this->points)
        bounding_rect.update(point);

    switch (point_count){
        case 1: type = shape_t::point;      break;
        case 2: type = shape_t::line;       break;
        case 3: type = shape_t::triangle;   break;
        case 4: type = shape_t::quad;       break;
        default:type = shape_t::polygon;    break;
    }
    
};



Shape Shape::create(shape_t type){
    std::vector<f2> points;
    float point_count;
    BoundingRect2D new_bounds;

    switch (type) {
    
    case shape_t::point:    point_count = 1.0f; break;
    case shape_t::line:     point_count = 2.0f; break;
    case shape_t::triangle: point_count = 3.0f; break;
    case shape_t::quad:     point_count = 4.0f; break;

    case shape_t::polygon:  point_count = 8.0f; break;
    
    }

    if(type == shape_t::point){
        points.emplace_back( f2{0.0f, 0.f} );
        return Shape{points};
    }
    else if(type == shape_t::line){
        points.emplace_back( f2{0.0f, 0.f} );
        points.emplace_back( f2{1.0f, 0.f} );
        return Shape{points};
    }

    float twoPI = 6.28f;
    float angle_step_size = twoPI / point_count;

    for(float th = 0.0f; th < twoPI; th += angle_step_size ){
        f2 new_point {cosf(th), sinf(th)};
        points.emplace_back( new_point );
    }

    return Shape{points};
}

bool Shape::is(shape_t type){
    return this->type == type ? true : false;
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


void Shape::set_bounds(BoundingRect2D new_bounding_rect){
    bounding_rect = new_bounding_rect;
}

void Shape::move(f2 move_vec){
    for(f2& point : points)
        point += move_vec;
    
    bounding_rect.move(move_vec);
}
void Shape::rotate(float theta){
    bounding_rect.reset();

    // for(f2& point : points){

    // }
        
    // bounding_rect.move(move_vec);
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
