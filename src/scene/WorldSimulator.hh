#ifndef WORLD_SIMULATOR_HH
#define WORLD_SIMULATOR_HH

#include <string>

#include "WorldObject.hpp"
#include "simulator/Simulator.hpp"


class WorldSimulator{
public:
    WorldSimulator(std::string _simname);

    std::string simname;


    void setSimulator(Sim::Simulator* _simulator);
    // WorldObject(std::string _modelName, std::string _objectName);
    void createSimContainer(std::string _modelName, std::string _objectName);

    void update();
    void render();
    
    Sim::Simulator* simulator = nullptr;

    WorldObject* simContainer = nullptr; // wireframe that will contain the simulation
    std::vector<WorldObject*> simulatorWorldObjects; // objects that are part of the simulation

    // Simulation scale / world scale
    float simulationScale = 0.1;
};






#endif