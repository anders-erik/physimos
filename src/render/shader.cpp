#include <glad/glad.h>

#include "shader.hpp"



Shader _worldShader;
unsigned int _transformLoc;
unsigned int _viewLoc;
unsigned int _sanityLoc;
unsigned int _perspectiveLoc;
unsigned int _colorLoc;
unsigned int _hasTextureLoc;

Shader _worldObjShader;
unsigned int _modelObjLoc;
unsigned int _viewObjLoc;
unsigned int _sanityObjLoc;
unsigned int _perspectiveObjLoc;

Shader _wireframeShader;
unsigned int _modelWireLoc;
unsigned int _viewWireLoc;
unsigned int _sanityWireLoc;
unsigned int _perspectiveWireLoc;


void shader_setWorldObject_uniforms(float* model_mat, float* view_mat, float* perspective_mat, const float* sanity_mat) {
    glUniformMatrix4fv(_modelObjLoc, 1, GL_TRUE, model_mat);
    glUniformMatrix4fv(_viewObjLoc, 1, GL_TRUE, view_mat);
    glUniformMatrix4fv(_sanityObjLoc, 1, GL_TRUE, sanity_mat);
    glUniformMatrix4fv(_perspectiveObjLoc, 1, GL_TRUE, perspective_mat);
}

void drawTriangles(int vertexCount){
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


Shader* getShader(Shaders _shaderType) {
    switch (_shaderType) {

    case Shaders::UI :
        return &_worldObjShader;
        break;
    case Shaders::worldObj:
        return &_worldObjShader;
        break;
    case Shaders::world:
        return &_worldShader;
        break;
    case Shaders::worldWireframe:
        return &_wireframeShader;
        break;
    
    default:
        return nullptr; // break if no match !
        break;
    }
}

void shader_init_shaders(){

    // char * shaderDir = "resources/shaders/";

    _worldShader.buildShaderProgram("resources/shaders/worldShader.vs", "resources/shaders/worldShader.fs");

    glUseProgram(_worldShader.ID);
    _transformLoc = glGetUniformLocation(_worldShader.ID, "transform");
    _viewLoc = glGetUniformLocation(_worldShader.ID, "view");
    _sanityLoc = glGetUniformLocation(_worldShader.ID, "sanityTransform");
    _perspectiveLoc = glGetUniformLocation(_worldShader.ID, "perspective");
    _colorLoc = glGetUniformLocation(_worldShader.ID, "vertexColor");
    _hasTextureLoc = glGetUniformLocation(_worldShader.ID, "hasTexture");
    // unsigned int _transformLoc;
    // unsigned int _viewLoc;
    // unsigned int _sanityLoc;
    // unsigned int _perspectiveLoc;
    // unsigned int _colorLoc;
    // unsigned int _hasTextureLoc;


    _worldObjShader.buildShaderProgram("resources/shaders/worldObjShader_vs.glsl", "resources/shaders/worldObjShader_fs.glsl");

    glUseProgram(_worldObjShader.ID);
    _modelObjLoc = glGetUniformLocation(_worldObjShader.ID, "model");
    _viewObjLoc = glGetUniformLocation(_worldObjShader.ID, "view");
    _sanityObjLoc = glGetUniformLocation(_worldObjShader.ID, "sanityTransform");
    _perspectiveObjLoc = glGetUniformLocation(_worldObjShader.ID, "perspective");

    _wireframeShader.buildShaderProgram("resources/shaders/worldWireframeShader_vs.glsl", "resources/shaders/worldWireframeShader_fs.glsl");

    glUseProgram(_wireframeShader.ID);
    _modelWireLoc = glGetUniformLocation(_wireframeShader.ID, "model");
    _viewWireLoc = glGetUniformLocation(_wireframeShader.ID, "view");
    _sanityWireLoc = glGetUniformLocation(_wireframeShader.ID, "sanityTransform");
    _perspectiveWireLoc = glGetUniformLocation(_wireframeShader.ID, "perspective");

}


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    Shader::buildShaderProgram(vertexPath, fragmentPath);

}




void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const float* mat4) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, mat4);

}

void Shader::use()
{
    glUseProgram(ID);
}


void Shader::buildShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram(); //Creates a program object
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
