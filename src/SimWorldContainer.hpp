
#ifndef SIM_WORLD_CONTAINER_HPP
#define SIM_WORLD_CONTAINER_HPP

#include <string>
#include <vector>

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
            if(simulator->simtype == Sim::Simtypes::VertThrow1)
                std::cout << "CONFIRMED SIMULATOR TYPE!"  << std::endl;
                
        }
        Sim::Simulator* simulator;

        std::vector<WorldObject> containerWorldObjects;
        float simulationScale = 0.1;

        void addSimulatorContainerVertices();

        // std::string name;
        
        void render();


};



#endif