
// #include "Types.hpp"
#include "Simulation.hpp"

// Holds the simulation object(s) and keeps the state of the simulation




// One timestep forward of 'dt' for SimObject
void updatePosAndVel(SimObject * so, float dt){


    // GRAVITY
    so->velocity.z = so->velocityPrevStep.z + (-9.8)*dt;

    so->translation.x = so->translationPrevStep.x + so->velocityPrevStep.x*dt;
    so->translation.y = so->translationPrevStep.y + so->velocityPrevStep.y*dt;
    so->translation.z = so->translationPrevStep.z + so->velocityPrevStep.z*dt;

    // BOUNCE
    // printf("%d\n", so->translation.y);
    if(so->translation.z < -20.0f){
        so->velocity.z = -so->velocity.z * 0.8;
        so->translation.z = -19.9f; // make sure the bounce condition is not met next 
        // printf("BOUNCE! \n");
    }

    // update prev step values
    so->velocityPrevStep.z = so->velocity.z;
    so->translationPrevStep.x = so->translation.x;
    so->translationPrevStep.y = so->translation.y;
    so->translationPrevStep.z = so->translation.z;
}




Simulation::Simulation(/* args */)
{
}

