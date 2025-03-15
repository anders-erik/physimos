#include "glad/glad.h"
#include "GLFW/glfw3.h"

// #include "asdf"

#include <iostream>
#include <fstream>
#include <sstream>



std::string cat(std::string path_str){
    std::ifstream _ifstream(path_str);

    std::stringstream _sstream;

    _sstream << _ifstream.rdbuf();

    return _sstream.str();

}

void framebuffer_callback(GLFWwindow* _window, int _width, int _height){
    glViewport(0, 0, _width, _height);
}


GLFWwindow* window = NULL;
int main(){

    glfwInit();
    window = glfwCreateWindow(600, 400, "HOLA", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, framebuffer_callback);

    std::string vert = cat("vert.gl");
    const char* vert_c = vert.c_str();

    std::string frag = cat("frag.gl");
    const char* frag_c = frag.c_str();

    // std::cout <<  frag << std::endl;

    int shader_success;

    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_c, NULL);
    glCompileShader(vert_shader);


    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &shader_success);
    if(!shader_success){
        std::cout << "ERROR COMPILING VERT SHADER." << std::endl;
        return 1;
    }


    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_c, NULL);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &shader_success);
    if(!shader_success){
        std::cout << "ERROR COMPILING FRAG SHADER." << std::endl;
        return 1;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &shader_success);
    if(!shader_success){
        std::cout << "ERROR LINKING SHADER PROGRAM." << std::endl;
        return 1;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glUseProgram(program);

    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    
    

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    std::cout << "Hello cube"  << std::endl;

}


