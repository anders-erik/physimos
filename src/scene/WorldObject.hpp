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
#include "render/shader.hpp"
#include "Model.hpp"
#include "RigidBody.hpp"


// librender
#include "render/wo_renderer.hpp"
#include "render/render_types.hh"

#include "transform.hh"


extern const float sanityMatrix16[16];

// enum WorldObjectType {
//     Default = 0,
//     SimWorldContainer = 1,
//     Ground = 2,
//     Sky = 3,
//     Light = 4,
// };


// struct wo_core {

// };



typedef struct WorldObject_Addons {
    bool hasModel = 0;
    objects::Model model;

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

// Forward declare testing
struct RigidBody;
struct RigidBody;
struct RigidBody;

class WorldObject {
public:
    WorldObject() {};
    WorldObject(std::string _modelName, std::string _objectName);

    std::string name;
    bool isActive = true;


    // NEW RENDER 2024-09-28
    bool isRendpipe = false;
    Shaders shaderType;
    void toggleWireframe();
    void update();
    objects::Model* model_ptr;
    WorldObject* parent = nullptr;
    std::vector<WorldObject*> children;

    Transform* transform_0 = nullptr; // initial transform (time = 0)
    Transform* transform = nullptr;

    Shader * shader;


    // Verts
    std::vector<float> vertices;
	int vertexCount;

    // PHYSICS
    RigidBody rigidBody;
    bool hasRigidBody = false;

    bool gravityOn = false;
    float offsetToBottom = 0.0f;
    BoundingBox boundingBox;
    std::vector<float> boundingVerts;


    // CALCULATED MODEL MATRIX
    void SetModelMatrixRowMajor_withParent();
    void SetModelMatrixRowMajor();
    float modelMatrixRowMajor[16] = { 0 };


    bool hasTransform = false;
    
    // MOVED TO 'TRANSFORM'
	Vec3 scale = {1.0f, 1.0f, 1.0f};

    Vec3 position_0 = { 0.0f, 0.0f, 0.0f };;
	Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 translationPrevStep = {0.0f, 0.0f, 0.0f};
    Vec3 velocity_0 = { 0.0f, 0.0f, 0.0f };;
    Vec3 velocityPrevStep = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};

    Vec3 rotation_0 = { 0.0f, 0.0f, 0.0f };;
	Vec3 rotation = {0.0f, 0.0f, 0.0f};
    Vec3 angularVelocity_0 = { 0.0f, 0.0f, 0.0f };;
    Vec3 angularVelocity = { 0.0f, 0.0f, 0.0f };
	
    



    

	

    // MOVED TO 'TRANSFORM'
    void Rotate(Vec3 rotationVector);
    void Translate(Vec3 moveVector);
    void SetPosition(Vec3 positionVector);
    void SetScale(Vec3 scaleVector);
    
    

    

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