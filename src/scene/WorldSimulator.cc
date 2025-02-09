
#include "WorldSimulator.hh"

#include "WorldObject.hpp"

WorldSimulator::WorldSimulator(std::string _simname) {
    simname = _simname;
}

void WorldSimulator::update(){
    // std::cout << "WorldSimulator::update()" << std::endl;
    simContainer->update();

    // step through all the simulator objects and update them using the simulator data
    for(WorldObject* _wo : simulatorWorldObjects){
        Sim::TimeStep* _timeStep = simulator->nextTimeStep();
        int sim_x = _timeStep->pos.x;
        int sim_y = _timeStep->pos.y;
        int sim_z = _timeStep->pos.z;
        
        // Assumes that the object being used in the simulator is zero-centered
        // Move object into sim-container, then shift according to simulator scale
        _wo->transform->position.data[0] = _wo->transform_0->position.data[0] + sim_x * simulationScale + simContainer->transform->position.data[0];
        _wo->transform->position.data[1] = _wo->transform_0->position.data[1] + sim_y * simulationScale + simContainer->transform->position.data[1];
        _wo->transform->position.data[2] = _wo->transform_0->position.data[2] + sim_z * simulationScale + simContainer->transform->position.data[2];

        // Scale the simulation realtive to container z-dimension + use Z
        _wo->transform->scale.data[0] = simContainer->transform->scale.data[0] * simulationScale;
        _wo->transform->scale.data[1] = simContainer->transform->scale.data[1] * simulationScale;
        _wo->transform->scale.data[2] = simContainer->transform->scale.data[2] * simulationScale;

        _wo->update();
    }
}



void WorldSimulator::createSimContainer(std::string _modelName, std::string _objectName) {
    simContainer = new WorldObject(_modelName, _objectName);
}

void WorldSimulator::setSimulator(Sim::Simulator* _simulator) {
    simulator = _simulator;
    if (simulator->simtype == Sim::Simtypes::VertThrow1){
        
    }
        // std::cout << "CONFIRMED SIMULATOR TYPE!" << std::endl;

}

