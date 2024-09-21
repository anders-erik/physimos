
#ifndef SIM_WORLD_CONTAINER_HPP
#define SIM_WORLD_CONTAINER_HPP

#include <string>

#include "WorldObject.hpp"
#include "Simulator.hpp"

class SimWorldContainer : public WorldObject {
    public:
        SimWorldContainer() {};
        // SimWorldContainer(Sim::Simulator* _simulator) {
        //     simulator = _simulator;
        // };
        // virtual ~SimWorldContainer() {};
        // SimWorldContainer(std::string name) {this->name = name;};
        void SetSimulator(Sim::Simulator* _simulator){
            simulator = _simulator;
        }
        Sim::Simulator* simulator;

        void addSimulatorVertices();
        // std::string name;
        
        void render();
};



#endif