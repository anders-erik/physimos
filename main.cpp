#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include <time.h>
#include <unistd.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void moveVerticies(float *verticies, int vertexCount);
void scale16Transform(float * transform16, float dx, float dy, float dz);
void setSimplePerspective(float * transformMatrix, float * perspectiveMatrix);
void translate16Transform(float * transform16, float dx, float dy, float dz);
float *  transposeToShader16(float * transform16, float * transformShader16);

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
	}
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// printf("%f , %f \n", xpos, ypos);
}


// SIMULATION





// settings
#define ZF 100.0f // far plane
#define ZN 1.0f // near plane

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

const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout(location = 0) in vec3 position;

	uniform mat4 transform;
	uniform mat4 perspective;

    void main()
    {
        gl_Position = perspective * transform * vec4(position, 1.0);
    }
)glsl";

const char *fragmentShaderSource = R"glsl(
    #version 330 core 

    out vec4 FragColor;
	uniform vec4 vertexColor; 

    void main()
    {
        FragColor = vertexColor;
    }
)glsl";

int main()
{

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

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//
	//		VERTEX DATA
	//
	
	
	//
	// GROUND
	//
	// Array for passing to shader
	float transform_shader[16];

	// float verticies[] = {
	// 	-0.1f, -0.1f, -0.9f, // far left
	// 	-0.9f, -0.9f, 0.9f,	 // near left
	// 	0.9f, -0.9f, 0.9f,	 // near right
	// 	0.1f, -0.1f, -0.9f,	 // far right
	// 	0.9f, -0.9f, 0.9f,	 // near right
	// 	-0.1f, -0.1f, -0.9f, // far left
	// };
	float verticies[] = {
		-1.0f, 0.1f, 1.0f, // far left
		-1.0f, 0.0f, -1.0f,	 // near left
		1.0f, 0.0f, -1.0f,	 // near right
		1.0f, 0.1f, 1.0f,	 // far right
		1.0f, 0.0f, -.01f,	 // near right
		-1.0f, 0.1f, 1.0f, // far left
	};
	float verticies_transform[16] = {
		 1.0f, 0.0f, 0.0f, 0.0f,  // top
		 0.0f, 1.0f, 0.0f, 0.0f, // left
		 0.0f, 0.0f, 1.0f, 0.0f,  // right
		 0.0f, 0.0f, 0.0f, 1.0f,  // right
	};
	float groundScale = 10.0f;
	scale16Transform(verticies_transform, groundScale, 1.0f, groundScale);
	translate16Transform(verticies_transform, 0.0f, -2.0f, 5.0f);

	//
	// TRIANGLE
	//
	float triangle[] = {
		0.0f,	 0.05f, 0.95f, // top
		-0.05f,	-0.05f, 0.95f, // left
		0.05f, 	-0.05f, 0.95f, // right
	};
	float triangleColor[] = {
		0.5, 0.0, 0.0, 1.0,
	};
	// float triangle_transform_4x4[4][4] = {
	// 	{ 1.0f, 0.0f, 0.0f, 0.0f, }, // top
	// 	{ 0.0f, 1.0f, 0.0f, 0.0f, },// left
	// 	{ 0.0f, 0.0f, 1.0f, 0.0f, }, // right
	// 	{ 0.0f, 0.0f, 1.0f, 0.0f, }, // right
	// };

	float triangle_transform[16] = {
		 1.0f, 0.0f, 0.0f, 0.0f,  // top
		 0.0f, 1.0f, 0.0f, 0.0f, // left
		 0.0f, 0.0f, 1.0f, 0.0f,  // right
		 0.0f, 0.0f, 0.0f, 1.0f,  // right
	};
	float triangleScale = 3.0f;
	scale16Transform(triangle_transform, triangleScale, triangleScale, 1.0f);
	translate16Transform(triangle_transform, 0.0f, 0.0f, 2.0f); // move triangle into simple projection area
	
	// float zf = ZF;
	// float zn = ZN;
	// float perspective_transform_simple[16] = {
	// 	 ZN  , 0.0f, 0.0f, 0.0f,  // top
	// 	 0.0f, ZN  , 0.0f, 0.0f, // left
	// 	 0.0f, 0.0f, ZN/(ZF-ZN), 0.0f,  // right
	// 	 0.0f, 0.0f, 0.0f, 1.0f,  // right
	// };
	float perspective_transform_simple[16] = {
		 1.0f, 0.0f, 0.0f, 0.0f,  // top
		 0.0f, 1.0f, 0.0f, 0.0f, // left
		 0.0f, 0.0f, 1.0f, 0.0f,  // right
		 0.0f, 0.0f, 0.0f, 1.0f,  // right
	};

	unsigned int ground_vao, ground_vbo;
	glGenVertexArrays(1, &ground_vao);
	glGenBuffers(1, &ground_vbo);

	unsigned int triangle_vao, triangle_vbo;
	glGenVertexArrays(1, &triangle_vao);
	glGenBuffers(1, &triangle_vbo);

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	unsigned int perspectiveLoc = glGetUniformLocation(shaderProgram, "perspective");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "vertexColor");

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// GROUND
	glBindVertexArray(ground_vao);

	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

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

	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(18 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// render loop
	// -----------
	clock_t lastFrameTime = 0;
	printf("CLOCKS_PER_SEC = %li \n", CLOCKS_PER_SEC);

    glfwTime = glfwGetTime();
	printf("glfwTime1 = %f \n", glfwTime);
	glfwTime = glfwGetTime();
	printf("glfwTime2 = %f \n", glfwTime);
	
	time(&epoch_fps);
	// time(&current_second_fps);
	while (!glfwWindowShouldClose(window))
	{
		// timespec_get( &waitForFrame , TIME_UTC);
		// printf("%d\n ", waitForFrame.tv_nsec);
		

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);

		//
		//
		// ADDED BY ME
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

		// moveVerticies(vertices, sizeof(float) * 12);
		// moveVerticies(ground_v, sizeof(float) * 12);

		glBindVertexArray(ground_vao);
		translate16Transform(verticies_transform, 0.0f, 0.0f, 0.01f);
		setSimplePerspective(verticies_transform, perspective_transform_simple);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transposeToShader16(verticies_transform, transform_shader));
		glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, perspective_transform_simple);
		glUniform4f(colorLoc, 0.0f, 0.5f, 0.0f, 1.0f); // https://learnopengl.com/Getting-started/Shaders
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(triangle_vao);
		translate16Transform(triangle_transform, -0.01f, 0.0f, 0.0f);
		setSimplePerspective(triangle_transform, perspective_transform_simple);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transposeToShader16(triangle_transform, transform_shader));
		glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, perspective_transform_simple);
		glUniform4f(colorLoc, 0.5f, 0.0f, 0.0f, 1.0f); // https://learnopengl.com/Getting-started/Shaders
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();


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

void moveVerticies(float *verticies, int vertexCount)
{
	for (int i = 0; i < vertexCount; i++)
	{
		verticies[i] = verticies[i] + 0.01;
	}
}

void scale16Transform(float * transform16, float dx, float dy, float dz){
	transform16[0]	*= dx;
	transform16[5]	*= dy;
	transform16[10]	*= dz;
}

void translate16Transform(float * transform16, float dx, float dy, float dz){
	transform16[3] += dx;
	transform16[7] += dy;
	transform16[11] += dz;
}

void setSimplePerspective(float * transformMatrix, float * perspectiveMatrix){
	perspectiveMatrix[0] = ZN / transformMatrix[11];
	perspectiveMatrix[5] = ZN / transformMatrix[11];
	perspectiveMatrix[10] = (ZN - transformMatrix[11]) / (ZF - ZN);
}

// 
float *  transposeToShader16(float * transform16, float * transformShader16){

	for(int c = 0; c < 4; c++){ // shader format column
		for(int r = 0; r < 4; r++){ // 
			transformShader16[c*4 + r] = transform16[r*4 + c];
		}
	}
	return transformShader16;

}