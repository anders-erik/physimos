// Holds the simulation object(s) and keeps the state of the simulation

#define X_0 0.0f
#define Y_0 -20.0f
#define Z_0 -50.0f

#define U_0 5.0f
#define V_0 30.0f
#define W_0 -5.0f

// Simulation constants
#define T0 0.0 
#define TF 10.0
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
	float transformMatrixRowMajor[16] = {0};
	
	Point3 position_0 = {X_0, Y_0, Z_0};
	Point3 velocity_0 = {U_0, V_0, W_0};
};


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
	SimObject simObject;

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

