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
#include "Model.hpp"
#include "RigidBody.hpp"

#include "wo_renderer.hpp"


// enum WorldObjectType {
//     Default = 0,
//     SimWorldContainer = 1,
//     Ground = 2,
//     Sky = 3,
//     Light = 4,
// };



typedef struct WorldObject_Addons {
    bool hasModel = 0;
    Model model;

    bool hasRigidBody = false;
    RigidBody rigidBody;

} WorldObject_Addons;


// enum WorldObjectType {
//     StaticModel = 0,
//     RigidBody_ = 1,
//     SimWorldContainer = 2,
//     Light = 3,
//     Sky = 4,
// };

// enum WorldObjectType {
//     StaticModel,
//     RigidBody_,
//     SimWorldContainer,
//     Light,
//     Sky
// };


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
    virtual ~WorldObject() = default;

    // virtual ~WorldObject() {};

    // WorldObjectType worldObjectType;

    std::string name;
    bool isActive = true;

    void LoadWorldObject(const char* path);


    // RENDER STUFF
    Renderer renderer;
    Shader * shader;
    void setShaderProgram(Shader* shader);
    unsigned int vao;
    unsigned int vbo;
    void setVaoVbo330();
    void setVaoVbo332();
    void setVaoVbo_obj();


    Model model;
    bool hasModel = false;

    // Texture
    int hasTexture = 0;
    unsigned int glTexture = 0;

    // Verts
    std::vector<float> vertices;
	int vertexCount;

    // PHYSICS
    RigidBody rigidBody;
    bool hasRigidBody = false;

    int gravityOn = 0;
    float offsetToBottom = 0.0f;
    BoundingBox boundingBox;
    std::vector<float> boundingVerts;


	Vec3 scale = {1.0f, 1.0f, 1.0f};

    Vec3 position_0;
	Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 translationPrevStep = {0.0f, 0.0f, 0.0f};
    Vec3 velocity_0;
    Vec3 velocityPrevStep = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};

    Vec3 rotation_0;
	Vec3 rotation = {0.0f, 0.0f, 0.0f};
    Vec3 angularVelocity_0;
    Vec3 angularVelocity = { 0.0f, 0.0f, 0.0f };
	
    
    float modelMatrixRowMajor[16] = {0};

    std::string modelPath = "";


    std::vector<WorldObject> children;

	
	
	



    // MATH
    void Rotate(Vec3 rotationVector);
    void Translate(Vec3 moveVector);
    void SetPosition(Vec3 positionVector);
    void SetScale(Vec3 scaleVector);
    void SetModelMatrixRowMajor();
    

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