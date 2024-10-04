
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
        _wo->transform->position.x = _wo->transform->position_0.x + sim_x * simulationScale + simContainer->transform->position.x;
        _wo->transform->position.y = _wo->transform->position_0.x + sim_y * simulationScale + simContainer->transform->position.y;
        _wo->transform->position.z = _wo->transform->position_0.x + sim_z * simulationScale + simContainer->transform->position.z;

        // Scale the simulation realtive to container z-dimension + use Z
        _wo->transform->scale.x = simContainer->transform->scale.z * simulationScale;
        _wo->transform->scale.y = simContainer->transform->scale.z * simulationScale;
        _wo->transform->scale.z = simContainer->transform->scale.z * simulationScale;

        _wo->update();
    }
}

void WorldSimulator::render() {
    // std::cout << "WorldSimulator::render()" << std::endl;
    simContainer->render();

    for (WorldObject* _wo : simulatorWorldObjects) {
        _wo->render();
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

