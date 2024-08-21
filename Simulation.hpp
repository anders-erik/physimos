// Holds the simulation object(s) and keeps the state of the simulation


// Simulation constants
#define T0 0.0 
#define TF 40.0
#define DT 0.1  
#define DT_COUNT (1 + (TF - T0) / DT ) 
#define DT_INDEX_MAX ((TF - T0) / DT)


enum SimState {
	    idle = 0,
	    startClickDetected = 1,
	    running = 2
    };


struct Point3 {
	float x;
	float y;
	float z;
};

// Turn into model
struct SimObject {
	float * vertices;
	int vertexCount;
	Point3 rotation = {0.0f, 0.0f, 0.0f};
	Point3 scale = {1.0f, 1.0f, 1.0f};
	Point3 translation = {0.0f, 0.0f, 0.0f};

    Point3 translationPrevStep = {0.0f, 0.0f, 0.0f};
    Point3 velocity = {0.0f, 0.0f, 0.0f};
    Point3 velocityPrevStep = {0.0f, 0.0f, 0.0f};
	float transformMatrixRowMajor[16] = {0};
	
	Point3 position_0;
	Point3 velocity_0;
};


// One timestep forward of 'dt' for SimObject
void updatePosAndVel(SimObject * so, float dt){
    // simple gravity
    so->velocity.y = so->velocityPrevStep.y + (-9.8)*dt;

    so->translation.x = so->translationPrevStep.x + so->velocityPrevStep.x*dt;
    so->translation.y = so->translationPrevStep.y + so->velocityPrevStep.y*dt;
    so->translation.z = so->translationPrevStep.z + so->velocityPrevStep.z*dt;

    // BOUNCE
    // printf("%d\n", so->translation.y);
    if(so->translation.y < -20.0f){
        so->velocity.y = -so->velocity.y * 0.8;
        so->translation.y = -19.9f; // make sure the bounce condition is not met next 
        // printf("BOUNCE! \n");
    }

    // update prev step values
    so->velocityPrevStep.y = so->velocity.y;
    so->translationPrevStep.x = so->translation.x;
    so->translationPrevStep.y = so->translation.y;
    so->translationPrevStep.z = so->translation.z;
}


class Simulation
{
private:
    
public:
    Simulation();

    

    SimState simState = idle;
	double t0 = T0;
	double tf = TF;
	double dt = DT;
	int dtCount = (int) DT_COUNT;
	int dtIndex = 0;
	int dtIndexMax = DT_INDEX_MAX;
	SimObject * simObject;

    // Simulation

    void setPositionAtT(SimObject * simObj, double t){
        simObj->translation.x = simObj->position_0.x + simObj->velocity_0.x * t;
        simObj->translation.y = simObj->position_0.y + simObj->velocity_0.y * t + 0.5 * (-9.8) * t * t;
        simObj->translation.z = simObj->position_0.z + simObj->velocity_0.z * t;
    }
};

Simulation::Simulation(/* args */)
{
}

