
#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>

#include "WorldObject.hpp"

class Simulator : public WorldObject {
    public:
        Simulator() {

        };
        // Simulator(std::string name) {this->name = name;};

        void addSimulatorVertices();
        // std::string name;
        
        void render();
};



#endif