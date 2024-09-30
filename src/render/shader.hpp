
#ifndef SHADER_H
#define SHADER_H



#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

typedef enum Shaders {
    UI,
    world,
    worldObj,
    worldWireframe
} Shaders;


// Shader _worldObjShader;
// unsigned int _modelObjLoc;
// unsigned int _viewObjLoc;
// unsigned int _sanityObjLoc;
// unsigned int _perspectiveObjLoc;
// typedef struct WorldObjShader_locations {
    
// }   WorldObjShader_locations;

void setHasTextureUniform(unsigned int _hasTexture);
void shader_setWorldObject_uniforms(float* model_mat, float* view_mat, float* perspective_mat, const float* sanity_mat);
void drawTriangles(int vertexCount);


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



Shader* getShader(Shaders _shaderType);
void shader_init_shaders();
  
 
#endif
