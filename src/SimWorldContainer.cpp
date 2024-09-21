
#include "SimWorldContainer.hpp"

#include "shapes.hpp"

// #include <iostream>


void SimWorldContainer::render(){
    // std::cout << "RENDERING simulaton. Name = " << this->name << std::endl;
    
}


void SimWorldContainer::addSimulatorVertices() {
    for(float vertexFloat : simulatorPoints){
        vertices.push_back(vertexFloat);
    }
    vertexCount = vertices.size();
}