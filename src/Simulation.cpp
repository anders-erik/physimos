
// #include "Types.hpp"
#include "Simulation.hpp"

#include "WorldObject.hpp"
extern WorldObject worldTriangle2_bounce;

#include "Input.hpp"
extern InputState inputState;

// Holds the simulation object(s) and keeps the state of the simulation

SimState simState = idle;

double t0 = T0;
double tf = TF;
double dt = DT;
int dtCount = (int)DT_COUNT;
int dtIndex = 0;
int dtIndexMax = DT_INDEX_MAX;
WorldObject* simWorldObject;



void updateSimulation(){
    if (simState == SimState::idle && inputState.startSimClick == 1) {
        simState = SimState::startClickDetected;
        inputState.startSimClick = 0;
    }
    else {
        // printf("Simulation already running! \n");
        inputState.startSimClick = 0;
    }

    // Start Simulation
    if (simState == SimState::startClickDetected) {
        simState = running;
        printf("Simulation Starting: \nsimulation.dtIndexMax = %d \n", dtIndexMax);

        // simulation.simObject = &tri1;
        // simulation.simObject->translationPrevStep = simulation.simObject->position_0;
        // simulation.simObject->velocityPrevStep = simulation.simObject->velocity_0;


        simWorldObject = &worldTriangle2_bounce;
        simWorldObject->translationPrevStep = simWorldObject->position_0;
        simWorldObject->velocityPrevStep = simWorldObject->velocity_0;

    }
    // stop and reset if stopping condition is met
    if (dtIndex >= dtIndexMax) {
        simState = SimState::idle;
        dtIndex = 0;
        // SetPositionSimObject(&tri1, tri1.position_0);

        simWorldObject->SetPosition(simWorldObject->position_0);
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
    simObj->position.x = simObj->position_0.x + simObj->velocity_0.x * t;
    simObj->position.y = simObj->position_0.y + simObj->velocity_0.y * t + 0.5 * (-9.8) * t * t;
    simObj->position.z = simObj->position_0.z + simObj->velocity_0.z * t;
}


// One timestep forward of 'dt' for SimObject
void sim_updatePosAndVel(WorldObject* wo) {


    // GRAVITY
    wo->velocity.z = wo->velocityPrevStep.z + (-9.8) * dt;

    wo->position.x = wo->translationPrevStep.x + wo->velocityPrevStep.x * dt;
    wo->position.y = wo->translationPrevStep.y + wo->velocityPrevStep.y * dt;
    wo->position.z = wo->translationPrevStep.z + wo->velocityPrevStep.z * dt;

    // BOUNCE
    // printf("%d\n", so->translation.y);
    if (wo->position.z < -20.0f) {
        wo->velocity.z = -wo->velocity.z * 0.8;
        wo->position.z = -19.9f; // make sure the bounce condition is not met next 
        // printf("BOUNCE! \n");
    }

    // update prev step values
    wo->velocityPrevStep.z = wo->velocity.z;
    wo->translationPrevStep.x = wo->position.x;
    wo->translationPrevStep.y = wo->position.y;
    wo->translationPrevStep.z = wo->position.z;
}




// ---- OLD BELOW -----



// // One timestep forward of 'dt' for SimObject
// void updatePosAndVel(SimObject * so, float dt){


//     // GRAVITY
//     so->velocity.z = so->velocityPrevStep.z + (-9.8)*dt;

//     so->translation.x = so->translationPrevStep.x + so->velocityPrevStep.x*dt;
//     so->translation.y = so->translationPrevStep.y + so->velocityPrevStep.y*dt;
//     so->translation.z = so->translationPrevStep.z + so->velocityPrevStep.z*dt;

//     // BOUNCE
//     // printf("%d\n", so->translation.y);
//     if(so->translation.z < -20.0f){
//         so->velocity.z = -so->velocity.z * 0.8;
//         so->translation.z = -19.9f; // make sure the bounce condition is not met next 
//         // printf("BOUNCE! \n");
//     }

//     // update prev step values
//     so->velocityPrevStep.z = so->velocity.z;
//     so->translationPrevStep.x = so->translation.x;
//     so->translationPrevStep.y = so->translation.y;
//     so->translationPrevStep.z = so->translation.z;
// }




// Simulation::Simulation(/* args */)
// {
// }

