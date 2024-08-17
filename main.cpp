#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include <time.h> 
#include <unistd.h>
#include <string.h>

#include "vertex.hpp"

// settings
#define ZF 100.0f // far plane
#define ZN 1.0f // near plane
// Simulation constants
#define T0 0.0
#define TF 10.0
#define DT 0.1
#define DT_COUNT (1 + (TF - T0) / DT )
#define DT_INDEX_MAX (TF - T0) / DT





const float perspectiveMatrix16_[16] = {
	ZN, 0, 0, 0,
	0, ZN, 0, 0,
	0, 0, -1.0f / (ZF - ZN), ZN / (ZF - ZN),
	0, 0, 1.0f, 0,
};
// http://www.songho.ca/opengl/gl_projectionmatrix.html
const float perspectiveMatrix16[16] = {
	ZN, 0, 0, 0,
	0, ZN, 0, 0,
	0, 0, - (ZF + ZN) / (ZF - ZN), - 2 * ZN * ZF / (ZF - ZN),
	0, 0, - 1.0f, 0,
};

enum SimState {
	idle = 0,
	startClickDetected = 1,
	running = 2
};



struct Simulation {
	SimState simState = idle;
	double t0 = T0;
	double tf = TF;
	double dt = DT;
	int dtCount = (int) DT_COUNT;
	int dtIndex = 0;
	int dtIndexMax = DT_INDEX_MAX;
	SimObject simObject;
} simulation;

// 	perspectiveMatrix[0] = ZN;
// 	perspectiveMatrix[5] = ZN;
// 	perspectiveMatrix[10] = -1.0f / (ZF - ZN);
// 	perspectiveMatrix[11] = ZN / (ZF - ZN); 
// 	perspectiveMatrix[14] = 1.0f; 

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

// void moveVerticies(float *verticies, int vertexCount);
// void scale16Transform(float * transform16, float dx, float dy, float dz);
// void setSimplePerspective(float * transformMatrix, float * perspectiveMatrix);
// void translate16Transform(float * transform16, float dx, float dy, float dz);
// float *  transposeToShader16(float * transform16, float * transformShader16);

// INPUT
struct Input {
	char s = 0;
	int pointerX = 0;
	int pointerY = 0; 
} input ;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
        printf("Click! -- %f , %f \n", xpos, ypos);
		if(simulation.simState == idle && ypos < 100.0 && xpos > 700.0){
			printf("Start Simulation button clicked! \n");
			simulation.simState = startClickDetected;
		}
	}
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// printf("%f , %f \n", xpos, ypos);
}
 

// SIMULATION







const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout(location = 0) in vec3 position;

	uniform mat4 transform;
	uniform mat4 perspective;

	out vec4 grayscaleColor;

    void main()
    {
		vec4 clipCoordinates = perspective * transform * vec4(position, 1.0);
		grayscaleColor = vec4( clipCoordinates.z / 100.0, clipCoordinates.z / 100.0,  clipCoordinates.z / 100.0, 1.0);

        gl_Position = clipCoordinates;
    }
)glsl";

const char *fragmentShaderSource = R"glsl(
    #version 330 core 

    out vec4 FragColor;
	uniform vec4 vertexColor; 

	in vec4 grayscaleColor;

    void main()
    {
		
        FragColor = grayscaleColor;
    }
)glsl";

int main() 
{
	printf("Length of Vshader: %lu \n", strlen(vertexShaderSource));
	printf("Length of Fshader: %lu \n", strlen(fragmentShaderSource));

	// vertexFunc();

	// segfault test
	// int *nlptr = NULL;
	// int a = *nlptr;
	
	// glfw: initialize and configure
	// -----------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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


	// glad: load all OpenGL function pointers  
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
 

	   

	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_NEVER);

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// glEnable(GL_DEPTH_TEST);  


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//
	//		VERTEX DATA
	//
	
	
	//
	// GROUND
	//
	float ground_vertices[] = {
		-1.0f, 0.0f, 1.0f, // far left 
		-1.0f, 0.0f, -1.0f,	 // near left
		1.0f, 0.0f, -1.0f,	 // near right
		1.0f, 0.0f, 1.0f,	 // far right
		1.0f, 0.0f, -1.0f,	 // near right
		-1.0f, 0.0f, 1.0f, // far left
	};
	
	struct SimObject ground1;
	ground1.vertices = ground_vertices;
	ground1.vertexCount = 6;
	
	float groundScale = 10.0f;
	struct Point3 ground1InitialPos = {10.0f, -20.0f, -35.0f};
	struct Point3 ground1InitialScale = {groundScale, 1.0f, groundScale};

	SetScaleSimObject(&ground1, ground1InitialScale);
	MoveSimObject(&ground1, ground1InitialPos); // move triangle into simple projection area




	//
	// TRIANGLE / OBJECT TO BE THROWN
	//
	float triangle[] = {
		0.0f,	 0.05f, 0.0f, // top
		-0.05f,	-0.05f, 0.0f, // left 
		0.05f, 	-0.05f, 0.0f, // right
	};
 

	struct SimObject tri1;
	tri1.vertices = triangle;
	tri1.vertexCount = 3;
	// tri1.translation = tri1.position_0;

	simulation.simObject = tri1;

	// printf("%f \n", *((tri1.vertices) + 1 ));
	float triangleScale = 20.0f;
	// struct Point3 tri1InitialRotation = {0.0f, 0.0f, 3.14f / 4 };
	struct Point3 tri1InitialScale = {triangleScale, triangleScale, 1.0f};
	struct Point3 tri1InitialPos = {X_0, Y_0, Z_0};

	// SetRotationSimObject(&tri1, tri1InitialRotation);
	SetScaleSimObject(&tri1, tri1InitialScale);
	MoveSimObject(&tri1, tri1InitialPos); // move triangle into simple projection area





	// VAO & VBOs

	unsigned int ground_vao, ground_vbo;
	glGenVertexArrays(1, &ground_vao);
	glGenBuffers(1, &ground_vbo);

	unsigned int triangle_vao, triangle_vbo;
	glGenVertexArrays(1, &triangle_vao);
	glGenBuffers(1, &triangle_vbo);

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	unsigned int perspectiveLoc = glGetUniformLocation(shaderProgram, "perspective");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "vertexColor");

	// GROUND
	glBindVertexArray(ground_vao);

	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// TRIANGLE

	glBindVertexArray(triangle_vao);

	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
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



		//
		// SIMULATION
		//

		// Start Simulation
		if(simulation.simState == startClickDetected){
			simulation.simState = running;
			printf("Simulation Starting: \nsimulation.dtIndexMax = %d \n", simulation.dtIndexMax);

			// glEnable(GL_DEPTH_TEST);
			// glDepthFunc(GL_LEQUAL); 

			// glDepthMask(GL_TRUE);
			// glEnable(GL_DEPTH_CLAMP);
			// glDepthRange(0.1, 2.0);
			// glClearDepth(1.0);
		}
		// stop and reset if stopping condition is met
		if (simulation.dtIndex >= simulation.dtIndexMax){
			simulation.simState = idle;
			simulation.dtIndex = 0;
			SetPositionSimObject(&tri1, tri1.position_0);
			printf("Simulation done. \n");

			// glClear(GL_DEPTH_BUFFER_BIT);
			// glDisable(GL_DEPTH_TEST);
		} 
		

		// Keep simulation running and check running condition
		if(simulation.simState == running && simulation.dtIndex < simulation.dtIndexMax){
			simulation.dtIndex++;
			printf("%d  ", simulation.dtIndex);

			setPositionAtT(&tri1, simulation.dtIndex*simulation.dt);
			// glClear(GL_DEPTH_BUFFER_BIT);
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
		glUseProgram(shaderProgram);
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
		
		RotationSimObject(&tri1, {0.0f, 0.0f, -0.01f});
		SetSimObjectTranform(&tri1);
		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, tri1.transformMatrixRowMajor);

		glUniform4f(colorLoc, 0.5f, 0.0f, 0.0f, 1.0f); // https://learnopengl.com/Getting-started/Shaders
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glClear(GL_DEPTH_BUFFER_BIT);


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

	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		printf("S pressed! \n");

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

