
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader() {};
    Shader(const char* vertexPath, const char* fragmentPath);

    void buildShaderProgram(const char* vertexPath, const char* fragmentPath);

    void use();

    // utility uniform functions

    // https://stackoverflow.com/questions/3141087/what-is-meant-with-const-at-end-of-function-declaration
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;
    
    void setFloat(const std::string &name, float value) const;

    void setMat4(const std::string &name, const float * mat4) const;


private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);
};
  
 
#endif
