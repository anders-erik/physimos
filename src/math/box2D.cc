
#include <iostream>

#include "box2D.hh"



Box2D Box2D::new_congruent_subbox(Box2D parent_box, Box2D child_box)
{

    float normalized_pos_x = child_box.pos.x / parent_box.size.x;
    float normalized_pos_y = child_box.pos.y / parent_box.size.y;

    float normalized_size_x = child_box.size.x / parent_box.size.x;
    float normalized_size_y = child_box.size.y / parent_box.size.y;

    f2 pos_norm  = {normalized_pos_x, normalized_pos_y};
    f2 size_norm = {normalized_size_x, normalized_size_y};


    Box2D prop_sub_box;

    prop_sub_box.pos.x = pos.x + pos_norm.x * size.x;
    prop_sub_box.pos.y = pos.y + pos_norm.y * size.y;

    prop_sub_box.size.x = size.x * size_norm.x;
    prop_sub_box.size.y = size.y * size_norm.y;

    return prop_sub_box;
}

bool Box2D::contains_point(f2 p)
{
    bool x_pass = p.x > pos.x && p.x < pos.x + size.x;
    bool y_pass = p.y > pos.y && p.y < pos.y + size.y;

    return x_pass && y_pass ? true : false;
}

void Box2D::print(){
    std::cout << "Box2D: " << std::endl;
    std::cout << "box.pos.x  = " << pos.x << std::endl;
    std::cout << "box.pos.y  = " << pos.y << std::endl;
    std::cout << "box.size.x = " << size.x << std::endl;
    std::cout << "box.size.y = " << size.y << std::endl;
    
}