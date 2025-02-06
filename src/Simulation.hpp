#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "Types.hpp"
#include "WorldObject.hpp"

// Holds the simulation object(s) and keeps the state of the simulation




enum SimState {
	    idle = 0,
	    startClickDetected = 1,
	    running = 2
};

void updateSimWithBouncyTriangle(WorldObject* bouncyTriangle);
void updateSimulation();

void sim_setPositionAtT(WorldObject* simObj, double t);
void sim_updatePosAndVel(WorldObject* wo);






// Turn into model
typedef struct SimObject {
	float * vertices;
	int vertexCount;
	Point3 rotation = {0.0f, 0.0f, 0.0f};
	Point3 scale = {1.0f, 1.0f, 1.0f};
	Point3 translation = {0.0f, 0.0f, 0.0f};

    Point3 translationPrevStep = {0.0f, 0.0f, 0.0f};
    Point3 velocity = {0.0f, 0.0f, 0.0f};
    Point3 velocityPrevStep = {0.0f, 0.0f, 0.0f};
	float modelMatrixRowMajor[16] = {0};
	
	Point3 position_0;
	Point3 velocity_0;
} SimObject;

// One timestep forward of 'dt' for SimObject
void updatePosAndVel(SimObject* so, float dt);



#endif