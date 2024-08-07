#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

// Vertex Shader source code
const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 color;
    out vec3 fragColor;
    void main()
    {
        fragColor = color;

        gl_Position = vec4(position, 1.0);
        
    }
)glsl";

// Fragment Shader source code
const char *fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 fragColor;
    out vec4 color;
    void main()
    {
        color = vec4(fragColor, 1.0);
    }
)glsl";

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        printf("'w' pressed! \n");
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW context to OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "Multiple Triangles", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Check maximum vertex attributes allowed on hardware
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Define vertices and colors
    GLfloat vertices[] = {
        // Positions         // Colors
        -0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Triangle 1
        -0.7f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.3f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // Triangle 2
        0.1f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        0.3f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Triangle 3
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.4f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f};

    // Define vertices and colors
    GLfloat movingTriangleVerticies[] = {
        0.1f,
        0.5f,
        0.0f,
        0.3f,
        0.5f,
        0.0f,
        0.2f,
        1.0f,
        0.0f,
    };
    GLfloat movingTriangleColors[] = {
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
    };

    // Vertex Array Object and Vertex Buffer Object
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(movingTriangleVerticies), movingTriangleVerticies, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(movingTriangleColors), movingTriangleColors, GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Vertex colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // MOVING TRIANGLE
    // positions
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(vertices)));
    // glEnableVertexAttribArray(2);
    // colors
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(vertices)+sizeof(movingTriangleVerticies)));
    // glEnableVertexAttribArray(3);

    // Unbind VAO
    glBindVertexArray(0);

    // Build and compile the shader program
    GLint success;
    GLchar infoLog[512];

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangles
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        glBindVertexArray(0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
