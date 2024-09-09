#ifndef WORLD_OBJECT_HPP
#define WORLD_OBJECT_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include <glad/glad.h>

#include "Types.hpp"
#include "shader.hpp"

typedef struct BoundingBox {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;
} BoundingBox;



class WorldObject {
public:
    WorldObject(const char* path);
    WorldObject();

    std::string name;

    void LoadWorldObject(const char* path);

    Shader * shader;
    void setShaderProgram(Shader* shader);
    unsigned int vao;
    unsigned int vbo;
    void setVaoVbo330();
    void setVaoVbo332();
    void setVaoVbo_obj();

    // Texture
    int hasTexture = 0;
    unsigned int glTexture = 0;
    // unsigned int* textureBuffer;


    // PHYSICS
    int gravityOn = 0;
    float offsetToBottom = 0.0f;
    BoundingBox boundingBox;
    std::vector<float> boundingVerts;


    std::vector<float> vertices;
	int vertexCount;

	Vec3 rotation = {0.0f, 0.0f, 0.0f};
	Vec3 position = {0.0f, 0.0f, 0.0f};
	Vec3 scale = {1.0f, 1.0f, 1.0f};
	float transformMatrixRowMajor[16] = {0};

    std::string modelPath = "";




    Vec3 translationPrevStep = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};
    Vec3 velocityPrevStep = {0.0f, 0.0f, 0.0f};
	
	Vec3 position_0;
	Vec3 velocity_0;



    // MATH
    void Rotate(Vec3 rotationVector);
    void Translate(Vec3 moveVector);
    void SetPosition(Vec3 positionVector);
    void SetScale(Vec3 scaleVector);
    void SetTransformMatrixRowMajor();
    

    void loadVerticexFromFile(); // Not currently in use !
    void printVertices();

    void printPosition();
    void printTransformMatrix();
    
};


class Cube : public WorldObject {
    public:
        Cube();
        BoundingBox boundingBox = { -1.0f, 1.0f,-1.0f, 1.0f, -1.0f, 1.0f};
        std::string cubeString = "CUBECUBE! \n";

};

#endif 