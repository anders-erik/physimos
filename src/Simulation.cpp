
// #include "Types.hpp"
#include "Simulation.hpp"

#include "WorldObject.hpp"
// extern WorldObject worldTriangle2_bounce;
extern WorldObject* worldTriangle2_simobj_pointer;

// WorldObject* worldTriangle2_bounce_pointer;

#include "Input.hpp"
// extern Input::InputState inputState;

// Holds the simulation object(s) and keeps the state of the simulation

SimState simState = idle;


// Simulation constants
#define T0 0.0
#define TF 30.0
#define DT 0.1  
#define DT_COUNT (1 + (TF - T0) / DT ) 
#define DT_INDEX_MAX ((TF - T0) / DT)

double t0 = T0;
double tf = TF;
double dt = DT;
int dtCount = (int)DT_COUNT;
int dtIndex = 0;
int dtIndexMax = DT_INDEX_MAX;
WorldObject* simWorldObject;


WorldObject* bouncyTrianglePointer;
void updateSimWithBouncyTriangle(WorldObject* bouncyTriangle){
    bouncyTrianglePointer = bouncyTriangle;
    updateSimulation();
}

void updateSimulation() {
    // WorldObject& worldTriangle2_bounce = *worldTriangle2_simobj_pointer;
    WorldObject& worldTriangle2_bounce = *bouncyTrianglePointer;
    

    if (simState == SimState::idle && PInput::inputState.startSimClick == 1) {
        simState = SimState::startClickDetected;
        PInput::inputState.startSimClick = 0;
    }
    else {
        // printf("Simulation already running! \n");
        PInput::inputState.startSimClick = 0;
    }

    // Start Simulation
    if (simState == SimState::startClickDetected) {
        simState = running;

        printf("Simulation Starting: \nsimulation.dtIndexMax = %d \n", dtIndexMax);

        // simulation.simObject = &tri1;
        // simulation.simObject->translationPrevStep = simulation.simObject->position_0;
        // simulation.simObject->velocityPrevStep = simulation.simObject->velocity_0;


        simWorldObject = &worldTriangle2_bounce;
        simWorldObject->transform->translationPrevStep = simWorldObject->transform->position_0;
        simWorldObject->transform->velocityPrevStep = simWorldObject->transform->velocity_0;

    }
    // stop and reset if stopping condition is met
    if (dtIndex >= dtIndexMax) {
        simState = SimState::idle;
        dtIndex = 0;
        // SetPositionSimObject(&tri1, tri1.position_0);

        simWorldObject->transform->SetPosition(simWorldObject->transform->position_0);
        printf("Simulation done. \n");

    }


    // Keep simulation running and check running condition
    if (simState == SimState::running && dtIndex < dtIndexMax) {

        dtIndex++;

        // updatePosAndVel(simulation.simObject, (float)simulation.dt);
        sim_updatePosAndVel(simWorldObject);


    }
}


void sim_setPositionAtT(WorldObject* simObj, double t) {
    simObj->transform->position.x = simObj->transform->position_0.x + simObj->transform->velocity_0.x * t;
    simObj->transform->position.y = simObj->transform->position_0.y + simObj->transform->velocity_0.y * t + 0.5 * (-9.8) * t * t;
    simObj->transform->position.z = simObj->transform->position_0.z + simObj->transform->velocity_0.z * t;
}


// One timestep forward of 'dt' for SimObject
void sim_updatePosAndVel(WorldObject* wo) {


    // GRAVITY
    wo->transform->velocity.z = wo->transform->velocityPrevStep.z + (-9.8) * dt;

    wo->transform->position.x = wo->transform->translationPrevStep.x + wo->transform->velocityPrevStep.x * dt;
    wo->transform->position.y = wo->transform->translationPrevStep.y + wo->transform->velocityPrevStep.y * dt;
    wo->transform->position.z = wo->transform->translationPrevStep.z + wo->transform->velocityPrevStep.z * dt;

    // BOUNCE
    // printf("%d\n", so->translation.y);
    if (wo->transform->position.z < 0.0f) {
        wo->transform->velocity.z = -wo->transform->velocity.z * 0.8;
        wo->transform->position.z = 0.01f; // make sure the bounce condition is not met next 
        // printf("BOUNCE! \n");
    }

    // update prev step values
    wo->transform->velocityPrevStep.z = wo->transform->velocity.z;
    wo->transform->translationPrevStep.x = wo->transform->position.x;
    wo->transform->translationPrevStep.y = wo->transform->position.y;
    wo->transform->translationPrevStep.z = wo->transform->position.z;
}


