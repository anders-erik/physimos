
#include "simulator.hpp"

#include "shapes.hpp"

// #include <iostream>


void Simulator::render(){
    // std::cout << "RENDERING simulaton. Name = " << this->name << std::endl;
    
}


void Simulator::addSimulatorVertices() {
    for(float vertexFloat : simulatorPoints){
        vertices.push_back(vertexFloat);
    }
    vertexCount = vertices.size();
}