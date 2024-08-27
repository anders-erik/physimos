#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <iostream>
#include <chrono>

#include <time.h> 
#include <unistd.h>
#include <string.h>

#include "Types.hpp"
#include "Simulation.hpp"
#include "vertex.hpp"
#include "shader.hpp"
#include "Camera.hpp"




Simulation simulation;
Camera camera;


/* 
	File Functions
*/
void renderUI();




const float sanityMatrix16[16] = {
	0, -1, 0, 0,
	0, 0, 1, 0,
	-1, 0, 0, 0,
	0, 0, 0, 1,
};

// Matrix mToggle_YZ = new Matrix(
// {1, 0, 0, 0}
// {0, 0, 1, 0}
// {0, 1, 0, 0}
// {0, 0, 0, 1})



const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// timing
#define TARGET_FPS 60
#define FRAME_DURATION (1000 / TARGET_FPS) // Duration of each frame in milliseconds

double glfwTime;

time_t epoch_fps, current_second_fps, current_time_fps, last_frame_fps;
int secondCount;
struct timespec wait = {0, 20000000L}; // nanoseconds of added wait between each frame
struct timespec waitForFrame = {0, 0L}; // nanoseconds of added wait between each frame



void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);




// INPUT
struct Input {
	// char s = 0;
	int pointerX = 0;
	int pointerY = 0;
	int mousePressActive = 0;
	int pointerXLastClick = 0;
	int pointerYLastClick = 0;
	int pointerXLastFrame = 0;
	int pointerYLastFrame = 0;
	// keys
	int middleMouse = 0;
	int ctrl = 0;
	int w = 0;
	int a = 0;
	int s = 0;
	int d = 0;
	int au = 0;
	int al = 0;
	int ad = 0;
	int ar = 0;
	
} input ;



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
        printf("Click! -- %f , %f \n", xpos, ypos);
		// printf("simstate: %d \n", SimState::idle);
		if(simulation.simState == SimState::idle && ypos < 100.0 && xpos > 700.0){
			printf("Start Simulation button clicked! \n");
			simulation.simState = SimState::startClickDetected;
		}
		input.mousePressActive = 1;
		input.pointerXLastClick = xpos;
		input.pointerYLastClick = ypos;
		
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		printf("RELEASED LEFT MOUSE BUTON\n");
		input.mousePressActive = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
		input.middleMouse = 1;
		input.pointerXLastFrame = xpos;
		input.pointerYLastFrame = ypos;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		input.middleMouse = 0;
	
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// printf("%f , %f \n", xpos, ypos);
	input.pointerX = xpos;
	input.pointerY = ypos;
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // printf("%d\n", key);
	// input.s = key;
	
	

	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) && action == GLFW_PRESS)
		input.ctrl = 1;
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)  && action == GLFW_RELEASE)
		input.ctrl = 0;
	

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		input.w = 1;
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		input.a = 1;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		input.s = 1;
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		input.d = 1;

	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		input.w = 0;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		input.a = 0;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		input.s = 0;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		input.d = 0;

	// ARROW KEYS
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		input.au = 1;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		input.al = 1;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		input.ad = 1;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		input.ar = 1;

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		input.au = 0;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		input.al = 0;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		input.ad = 0;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		input.ar = 0;
}
 







int main() 
{
	// printf("Length of Vshader: %lu \n", strlen(vertexShaderSource));
	// printf("Length of Fshader: %lu \n", strlen(fragmentShaderSource));

	// vertexFunc();

	// segfault test
	// int *nlptr = NULL; 
	// int a = *nlptr;
	
	// glfw: initialize and configure
	// -----------------------------
	glfwInit();
	// glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	
	// glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
  
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
 
	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLThrowSim", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
 
	//
	// INPUT
	//
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);


	// glad: load all OpenGL function pointers  
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
 
	// std::cout << "ERROR ? :" << std::endl;
	// std::cout << glGetError() << std::endl;

	
	// build and compile our shader zprogram
    // ------------------------------------
    Shader worldShader("shader.vs", "shader.fs");
	
	Shader uiShader("ui.vs", "ui.fs");



	/* 
		SIMULATION SETUP
	*/
	// Creates a new instance of simulation, leading to some defined pointers to break
	// Simulation simulation = Simulation();


	// CAMERA SETUP
	//
	//
	


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//
	//		VERTEX DATA
	//	
	
	//
	// GROUND
	//
	// x-y - vieved from above - x right, y up
	// float ground_vertices[] = {
	// 	-1.0f, 1.0f, 0.0f, // top left
	// 	-1.0f, -1.0f, 0.0f,	 // bottom left
	// 	1.0f, -1.0f, 0.0f,	 // botton right
	// 	1.0f, 1.0f, 0.0f,	 // top right
	// 	1.0f, -1.0f, 0.0f,	 // bottom right
	// 	-1.0f, 1.0f, 0.0f, // top left
	// };
	float ground_vertices[] = {
		// bottom left triangle
		1.0f, 1.0f, 0.0f, 	0.3f, 0.0f, 0.0f, 	 // top left
		-1.0f, 1.0f, 0.0f, 	0.3f, 0.0f, 0.0f,	 // bottom left
		-1.0f, -1.0f, 0.0f, 0.3f, 0.0f, 0.0f,	 // botton right
		// top right triangle
		1.0f, -1.0f, 0.0f, 	0.3f, 0.0f, 0.0f,	 // top right
		1.0f, 1.0f, 0.0f, 	0.3f, 0.0f, 0.0f, 	 // top left
		-1.0f, -1.0f, 0.0f, 0.3f, 0.0f, 0.0f,	 // bottom right
	};

	// y - z plane
	// float ground_vertices[] = {
	// 	-1.0f, 0.0f, 1.0f, // top left
	// 	-1.0f, 0.0f, -1.0f,	 // bottom left
	// 	1.0f, 0.0f, -1.0f,	 // botton right
	// 	1.0f, 0.0f, 1.0f,	 // top right
	// 	1.0f, 0.0f, -1.0f,	 // bottom right
	// 	-1.0f, 0.0f, 1.0f, // top left
	// };

	struct SimObject ground1;
	ground1.vertices = ground_vertices;
	ground1.vertexCount = 6;
	
	float groundScale = 50.0f;
	struct Point3 ground1InitialPos = {50.0f, 0.0f, -20.0f};
	struct Point3 ground1InitialScale = {groundScale, groundScale, 1.0f};

	SetScaleSimObject(&ground1, ground1InitialScale);
	MoveSimObject(&ground1, ground1InitialPos); // move triangle into simple projection area



	//
	// TRIANGLE / OBJECT TO BE THROWN
	//

	//	x-y
	// float triangle[] = {
	// 	0.0f,	 0.05f, 0.0f, // top
	// 	-0.05f,	-0.05f, 0.0f, // left 
	// 	0.05f, 	-0.05f, 0.0f, // right
	// }; 

	// y-z 
	float triangle[] = {
		0.0f, 0.0f, 0.05f, 		0.0f, 1.0f, 0.0f, // top
		0.0f, 0.05f, -0.05f, 	0.0f, 1.0f, 0.0f, // left 
		0.0f, -0.05f, -0.05f, 	0.0f, 1.0f, 0.0f, // right
	}; 
 

	struct SimObject tri1;
	tri1.vertices = triangle;
	tri1.vertexCount = 3;
	// tri1.translation = tri1.position_0;

	simulation.simObject = &tri1;

	// printf("%f \n", *((tri1.vertices) + 1 ));
	float triangleScale = 20.0f;

	// struct Point3 tri1InitialRotation = {0.0f, 0.0f, 3.14f / 4 };
	struct Point3 tri1InitialScale = {1.0f, triangleScale, triangleScale};
	struct Point3 tri1InitialPos = {45.0f, 30.0f, -0.0f};
	struct Point3 tri1InitialVel = {-1.0f, -0.5f, 25.0f};
	tri1.position_0 = tri1InitialPos;
	tri1.velocity_0 = tri1InitialVel;

	// SetRotationSimObject(&tri1, tri1InitialRotation);
	SetScaleSimObject(&tri1, tri1InitialScale);
	MoveSimObject(&tri1, tri1InitialPos); // move triangle into simple projection area




	//
	// CUBE
	//
	// y-z 
	float cube1_vertices[] = {
		// look + x for correct face reckoing
		// Front face
		-1.0f, 1.0f, 1.0f, 	0.0f, 0.0f, 1.0f, // top left
		-1.0f, 1.0f, -1.0f, 	0.0f, 0.0f, 1.0f, // botton left
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f, // bottom right

		-1.0f, -1.0f, 1.0f, 	0.0f, 0.0f, 1.0f, // top right
		-1.0f, 1.0f, 1.0f, 	0.0f, 0.0f, 1.0f, // top left
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom right
		// Back Face
		1.0f, 1.0f, 1.0f, 	1.0f, 0.0f, 1.0f, // top left
		1.0f, 1.0f, -1.0f, 	1.0f, 0.0f, 1.0f, // botton left
		1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 1.0f, // bottom right

		1.0f, -1.0f, 1.0f, 	1.0f, 0.0f, 1.0f, // top right
		1.0f, 1.0f, 1.0f, 	1.0f, 0.0f, 1.0f, // top left
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, // bottom right

		// Right Face - 							look +y
		-1.0f, -1.0f,  1.0f, 	0.0f, 1.0f, 1.0f, // top left
		-1.0f, -1.0f, -1.0f, 	0.0f, 1.0f, 1.0f, // botton left
		 1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 1.0f, // bottom right

		 1.0f, -1.0f,  1.0f, 	0.0f, 1.0f, 1.0f, // top right
		-1.0f, -1.0f,  1.0f, 	0.0f, 1.0f, 1.0f, // top left
		 1.0f, -1.0f, -1.0f, 	0.0f, 1.0f, 1.0f, // bottom right
		 // Left Face - 							look +y
		-1.0f, 1.0f,  1.0f, 	1.0f, 1.0f, 0.0f, // top left
		-1.0f, 1.0f, -1.0f, 	1.0f, 1.0f, 0.0f, // botton left
		 1.0f, 1.0f, -1.0f,		1.0f, 1.0f, 0.0f, // bottom right

		 1.0f, 1.0f,  1.0f, 	1.0f, 1.0f, 0.0f, // top right
		-1.0f, 1.0f,  1.0f, 	1.0f, 1.0f, 0.0f, // top left
		 1.0f, 1.0f, -1.0f, 	1.0f, 1.0f, 0.0f, // bottom right

		 // Bottom Face - 							look +z, +x i right, -y top
		-1.0f, -1.0f, -1.0f, 	0.0f, 0.5f, 1.0f, // top left
		-1.0f,  1.0f, -1.0f, 	0.0f, 0.5f, 1.0f, // botton left
		 1.0f,  1.0f, -1.0f,	0.0f, 0.5f, 1.0f, // bottom right

		 1.0f, -1.0f, -1.0f, 	0.0f, 0.5f, 1.0f, // top right
		-1.0f, -1.0f, -1.0f, 	0.0f, 0.5f, 1.0f, // top left
		 1.0f,  1.0f, -1.0f, 	0.0f, 0.5f, 1.0f, // bottom right

		 // Bottom Face - 							look +z, +x i right, -y top
		-1.0f, -1.0f,  1.0f, 	1.0f, 0.5f, 0.0f, // top left
		-1.0f,  1.0f,  1.0f, 	1.0f, 0.5f, 0.0f, // botton left
		 1.0f,  1.0f,  1.0f,	1.0f, 0.5f, 0.0f, // bottom right

		 1.0f, -1.0f,  1.0f, 	1.0f, 0.5f, 0.0f, // top right
		-1.0f, -1.0f,  1.0f, 	1.0f, 0.5f, 0.0f, // top left
		 1.0f,  1.0f,  1.0f, 	1.0f, 0.5f, 0.0f, // bottom right
	}; 
 

	struct SimObject cube1;
	cube1.vertices = triangle;
	cube1.vertexCount = 36;

	float cube1Scale = 1.0f;

	// struct Point3 tri1InitialRotation = {3.14f / 4 , 0.0f, 0.0f};
	struct Point3 cube1InitialScale = {1.0f, cube1Scale, cube1Scale};
	struct Point3 cube1InitialPos = {5.0f, 0.0f, 0.0f};
	// struct Point3 cube1InitialVel = {-1.0f, 0.1f, 25.0f};
	struct Point3 cube1InitialRotation = {0.0f, 0.0f, 0.75f};
	tri1.position_0 = tri1InitialPos;
	tri1.velocity_0 = tri1InitialVel;

	SetRotationSimObject(&cube1, cube1InitialRotation);
	SetScaleSimObject(&cube1, cube1InitialScale);
	MoveSimObject(&cube1, cube1InitialPos); // move triangle into simple projection area





	// VAO & VBOs

	unsigned int ground_vao, ground_vbo;
	glGenVertexArrays(1, &ground_vao);
	glGenBuffers(1, &ground_vbo);


	unsigned int transformLoc = glGetUniformLocation(worldShader.ID, "transform");
	unsigned int viewLoc = glGetUniformLocation(worldShader.ID, "view");
	unsigned int sanityLoc = glGetUniformLocation(worldShader.ID, "sanityTransform");
	unsigned int perspectiveLoc = glGetUniformLocation(worldShader.ID, "perspective");
	unsigned int colorLoc = glGetUniformLocation(worldShader.ID, "vertexColor");

	// GROUND
	glBindVertexArray(ground_vao);

	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// TRIANGLE

	unsigned int triangle_vao, triangle_vbo;
	glGenVertexArrays(1, &triangle_vao);
	glGenBuffers(1, &triangle_vbo);

	glBindVertexArray(triangle_vao);

	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// CUBE 1
	unsigned int cube1_vao, cube1_vbo;
	glGenVertexArrays(1, &cube1_vao);
	glGenBuffers(1, &cube1_vbo);
	
	glBindVertexArray(cube1_vao);

	glBindBuffer(GL_ARRAY_BUFFER, cube1_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube1_vertices), cube1_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);


	// INITIAL CLOCK INFO
	clock_t lastFrameTime = 0;
	printf("CLOCKS_PER_SEC = %li \n", CLOCKS_PER_SEC);

    glfwTime = glfwGetTime();
	printf("glfwTime1 = %f \n", glfwTime);
	glfwTime = glfwGetTime();
	printf("glfwTime2 = %f \n", glfwTime);
	
	time(&epoch_fps);

 


	// render loop
	// -----------
	// glDepthMask(GL_TRUE);
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_ALWAYS); // always passes test - i.e. no depth test done 
	while (!glfwWindowShouldClose(window)) 
	{
		// timespec_get( &waitForFrame , TIME_UTC);
		// printf("%d\n ", waitForFrame.tv_nsec);

		// glDepthRange(1.0, 0.0); 
		glEnable(GL_DEPTH_TEST);

		//
		// SIMULATION
		//

		// Start Simulation
		// printf("simstate: %d \n", simulation.simState);
		if(simulation.simState == SimState::startClickDetected){
			simulation.simState = running;
			printf("Simulation Starting: \nsimulation.dtIndexMax = %d \n", simulation.dtIndexMax);

			simulation.simObject = &tri1;

			simulation.simObject->translationPrevStep = simulation.simObject->position_0;
			simulation.simObject->velocityPrevStep = simulation.simObject->velocity_0;

			
			// glClearDepth(1.0);
			// glDepthFunc(GL_LEQUAL); 
 
			// glDepthMask(GL_TRUE);
			// glEnable(GL_DEPTH_CLAMP);
			
			// glClearDepth(1.0);

		}
		// stop and reset if stopping condition is met
		if (simulation.dtIndex >= simulation.dtIndexMax){
			simulation.simState = SimState::idle;
			simulation.dtIndex = 0; 
			SetPositionSimObject(&tri1, tri1.position_0);
			printf("Simulation done. \n");

			// glClear(GL_DEPTH_BUFFER_BIT);
			// glDisable(GL_DEPTH_TEST); 
		} 
		

		// Keep simulation running and check running condition
		if(simulation.simState == SimState::running && simulation.dtIndex < simulation.dtIndexMax){
			
			simulation.dtIndex++;
			
			updatePosAndVel(simulation.simObject, (float)simulation.dt);
			

			// printf("%f \n", simulation.simObject->translation.z);


			// printf("%d ", simulation.dtIndex);

			// Run next time step

			// simulation.setPositionAtT(&tri1, simulation.dtIndex*simulation.dt); 
		}
		
 



		// input
		// -----
		processInput(window);
 
		// render
		// ------
		// glEnable(GL_DEPTH_TEST);  
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		

		// glDepthMask(GL_TRUE);
		

		// set program and perspective-matrix
		glUseProgram(worldShader.ID);
		glUniformMatrix4fv(perspectiveLoc, 1, GL_TRUE, perspectiveMatrix16);

		//
		// FPS INFO
		//

		// time(&last_frame_fps);
		time(&current_time_fps);
		++secondCount;
		// entered new second
		if (current_time_fps > current_second_fps)
		{
			current_second_fps = current_time_fps;
			printf("FPS: %d \n", secondCount);
			// printf("%d \n", (int)(current_time - epoch_time));
			secondCount = 0;
		}



		// SANITY MATRIX
		glUniformMatrix4fv(sanityLoc, 1, GL_TRUE, sanityMatrix16);

		


		// CAMERA
		float forwardX = cos(camera.eulerAnglesRad.c);
		float forwardY = sin(camera.eulerAnglesRad.c);

		if(input.w)
			camera.translate(forwardX*0.2f, forwardY*0.2f, 0.0f);
		if(input.s)
			camera.translate(-forwardX*0.2f, -forwardY*0.2f, 0.0f);
		if(input.a)
			camera.translate(-forwardY*0.2f, forwardX*0.2f, 0.0f);
		if(input.d)
			camera.translate(forwardY*0.2f, -forwardX*0.2f, 0.0f);

		if(input.au)
			camera.rotateEulerRad(0.0f, 0.05f, 0.0f);
		if(input.ad)
			camera.rotateEulerRad(0.0f, -0.05f, 0.0f);
		if(input.al)
			camera.rotateEulerRad(0.0f, 0.0f, 0.05f);
		if(input.ar)
			camera.rotateEulerRad(0.0f, 0.0f, -0.05f);


			// printf("mouse + ctrl\n");
		// if(input.mousePressActive && input.ctrl){
		if(input.middleMouse){
			float dx = input.pointerX - input.pointerXLastFrame;
			float dy = input.pointerY - input.pointerYLastFrame;

			input.pointerXLastFrame = input.pointerX;
			input.pointerYLastFrame = input.pointerY;

			// printf("%f\n", dx);
			// printf("%f\n", dy);

			camera.rotateEulerRad(0.0f, 0.0f, -dx * 0.005f);
			camera.rotateEulerRad(0.0f, -dy * 0.005f, 0.0f);

		}

		camera.rotateEulerRad(0.0f, 0.0f, 0.0f);
		camera.setViewMatrix();
		// printf("Camera Position: %f", camera.cameraPosition.x);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, camera.viewMatrix);
		// glUniformMatrix4fv(viewLoc, 1, GL_TRUE, viewMatrix16);



 
		// GROUND
		glBindVertexArray(ground_vao);

		// struct Point3 moveGround1 = {0.0f, -0.01f, 0.1f};
		// MoveSimObject(&ground1, moveGround1);

		SetSimObjectTranform(&ground1);
		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, ground1.transformMatrixRowMajor);
		
		glUniform4f(colorLoc, 0.0f, 0.5f, 0.0f, 1.0f); // https://learnopengl.com/Getting-started/Shaders
		glDrawArrays(GL_TRIANGLES, 0, 6);
		

		// glClear(GL_DEPTH_BUFFER_BIT);
		 
		// TRIANGLE
		glBindVertexArray(triangle_vao);

		// Dynamic movement 
		// struct Point3 rotTri1 = {0.0f, 0.0f, 0.01f};
		// RotationSimObject(&tri1, rotTri1);
		// struct Point3 moveTri1 = {0.01f, 0.0f, 0.0f};
		// MoveSimObject(&tri1, moveTri1);
		
		// RotationSimObject(simulation.simObject, {0.0f, 0.0f, -0.01f});
		// SetSimObjectTranform(&tri1);
		SetSimObjectTranform(simulation.simObject);
		// glUniformMatrix4fv(transformLoc, 1, GL_TRUE, tri1.transformMatrixRowMajor);
		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, simulation.simObject->transformMatrixRowMajor);
		// glUniformMatrix4fv(sanityLoc, 1, GL_TRUE, sanityMatrix16);

		glUniform4f(colorLoc, 0.5f, 0.0f, 0.0f, 1.0f); // https://learnopengl.com/Getting-started/Shaders
		glDrawArrays(GL_TRIANGLES, 0, 3);

		
		// CUBE 1
		glBindVertexArray(cube1_vao);

		RotationSimObject(&cube1, {0.03f, 0.01f, 0.0f});
		SetSimObjectTranform(&cube1);

		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, cube1.transformMatrixRowMajor);
		// glUniformMatrix4fv(sanityLoc, 1, GL_TRUE, sanityMatrix16);

		glUniform4f(colorLoc, 0.5f, 0.0f, 0.0f, 1.0f); // https://learnopengl.com/Getting-started/Shaders
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/* 
			Render UI
		*/
		glUseProgram(uiShader.ID);
		renderUI();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

		// Keep track of time since last frame and wait to match the target frame rate
		clock_t currentTime = clock();
		double timeTaken = (double)(currentTime - lastFrameTime) / CLOCKS_PER_SEC * 1000.0;

		if (timeTaken < FRAME_DURATION)
		{
			usleep((FRAME_DURATION - timeTaken) * 1000); // Sleep for the remaining time
		}

		lastFrameTime = clock();

		// nanosleep(&wait, &wait); // old method
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &ground_vao);
	glDeleteBuffers(1, &ground_vbo);

	glDeleteProgram(worldShader.ID);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	// glfwDestroyWindow(window)
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	// 	printf("S pressed! \n");
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int uiVAO = 0;
unsigned int uiVBO = 0;
void renderUI()
{
    // initialize (if necessary)
    if (uiVAO == 0)
    {
        float vertices[] = {
            // back face
            0.7f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-left
            1.0f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f, // top-right
            1.0f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-right         
            1.0f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f, // top-right
            0.7f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-left
            0.7f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f,  // top-left

			// Play triangle
			0.8f, 0.9f, -1.0f,  0.0f,  1.0f, 0.0f, // top-left
            0.8f,  0.8f, -1.0f,  0.0f,  1.0f, 0.0f, // botton-left
            0.9f, 0.85f, -1.0f,  0.0f,  1.0f, 0.0f, // middle-right
        };
        glGenVertexArrays(1, &uiVAO);
        glGenBuffers(1, &uiVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(uiVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(uiVAO);
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glBindVertexArray(0);
}
