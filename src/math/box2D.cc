
#include <iostream>

#include "box2D.hh"

void Box2D::print(){
    std::cout << "Box2D: " << std::endl;
    std::cout << "box.pos.x  = " << pos.x << std::endl;
    std::cout << "box.pos.y  = " << pos.y << std::endl;
    std::cout << "box.size.x = " << size.x << std::endl;
    std::cout << "box.size.y = " << size.y << std::endl;
    
}