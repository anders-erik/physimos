
#include <iostream>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "math/math.hh"

#include "phont.hh"


struct Vertex {
    f3 pos;
    f2 tex;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}

// settings
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;


const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec2 aTextCoord;

    out vec2 textCoord;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);

        textCoord = aTextCoord;
    }
)glsl";

const char *fragmentShaderSource = R"glsl(
    #version 330 core 

    uniform sampler2D myTexture;

    in vec2 textCoord;

    out vec4 FragColor;
    void main()
    {
        vec4 textColor = texture(myTexture, textCoord);

        // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        FragColor = textColor;
    }
)glsl";

int main()
{
    f3 _f3;

    std::cout << "_f3.x = " << _f3.x << std::endl;

    phont::hello();


	glfwInit();
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

    glEnable(GL_MULTISAMPLE);

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




    // QUAD

    std::array<Vertex, 4> quad;
    std::array<i3, 2> faces;

    float width_pixels  = 200.0f;
    float height_pixels = 300.0f;
    
	float x_center_ndc = 0.0f;
    float y_center_ndc = 0.0f;



    float width_ndc_per_pixel = (1.0f / WINDOW_WIDTH);
    float height_ndc_per_pixel = (1.0f / WINDOW_HEIGHT);

    float width_ndc = width_pixels * width_ndc_per_pixel;
    float height_ndc = height_pixels * height_ndc_per_pixel;


    // Lower left
    quad[0].pos.x = x_center_ndc - width_ndc / 2;
    quad[0].pos.y = y_center_ndc - height_ndc / 2;
    quad[0].pos.z = 0.0f;
    quad[0].tex.x = 0.0f;
    quad[0].tex.y = 0.0f;

    // Lower right
    quad[1].pos.x = x_center_ndc + width_ndc / 2;
    quad[1].pos.y = y_center_ndc - height_ndc / 2;
    quad[1].pos.z = 0.0f;
    quad[1].tex.x = 1.0f;
    quad[1].tex.y = 0.0f;

    // Upper right
    quad[2].pos.x = x_center_ndc + width_ndc / 2;
    quad[2].pos.y = y_center_ndc + height_ndc / 2;
    quad[2].pos.z = 0.0f;
    quad[2].tex.x = 1.0f;
    quad[2].tex.y = 1.0f;

    // Upper left
    quad[3].pos.x = x_center_ndc - width_ndc / 2;
    quad[3].pos.y = y_center_ndc + height_ndc / 2;
    quad[3].pos.z = 0.0f;
    quad[3].tex.x = 0.0f;
    quad[3].tex.y = 1.0f;
    

    faces[0] = {0, 1, 2};
    faces[1] = {0, 2, 3};


	// float verts_quad[] = {

	// 	-0.80f, -0.90f, 0.0f, 0.0f, 0.0f, // b left
	// 	 0.80f, -0.90f, 0.0f, 1.0f, 0.0f, // b right
	// 	 0.0f,   0.95f, 0.0f, 0.5f, 1.0f, // t left

	// };

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(i3), faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad.data(), GL_STATIC_DRAW);
    

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(f3)));
	glEnableVertexAttribArray(1);



	glBindBuffer(GL_ARRAY_BUFFER, 0);



    // TEXTURE
    unsigned int texture_checker = 0;
    phont::set_texture(texture_checker);



	while (!glfwWindowShouldClose(window))
	{

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		    glfwSetWindowShouldClose(window, true);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderProgram);

        glBindTexture(GL_TEXTURE_2D, texture_checker);

		glBindVertexArray(VAO); 
		// glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, faces.size() * 3 , GL_UNSIGNED_INT, 0);

		glBindVertexArray(0); 


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteProgram(shaderProgram);


	glfwTerminate();
	return 0;
}




