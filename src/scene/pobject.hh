#ifndef WORLD_OBJECT_HPP
#define WORLD_OBJECT_HPP

#include <vector>
#include <string>


#include "Model.hpp"
#include "RigidBody.hpp"
#include "transform.hh"


// librender
#include "rend/shader.hpp"
#include "rend/wo_renderer.hpp"
#include "rend/render_types.hh"



extern const float sanityMatrix16[16];


// will be moved to physicsbody
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

class PObject {
public:

    std::string name;
    bool isActive = true;


    /*
        Render
     */
    Shader* shader;
    Shaders shaderType;
    void toggleWireframe();
    void update();
    objects::Model* model_ptr;
    PObject* parent = nullptr;
    std::vector<PObject*> children;


    /* 
        Transforms
     */
    // initial timestep transform (time = 0)
    Transform* transform_0 = nullptr;
    // Previous timestep transform (time = n-1)
    Transform* transform_prev = nullptr;
    // Current timestep transform (time = n)
    Transform* transform = nullptr;

    // CALCULATED MODEL MATRIX
    void SetModelMatrixRowMajor_withParent();
    void SetModelMatrixRowMajor();
    float modelMatrixRowMajor[16] = { 0 };
    void printTransformMatrix();


    /*
        Physics
     */
    RigidBody rigidBody;
    bool hasRigidBody = false;
    bool gravityOn = false;
    float offsetToBottom = 0.0f;
    BoundingBox boundingBox;
    std::vector<float> boundingVerts;
    

    /*
        Constructors
     */
    PObject() {};
    PObject(std::string _modelName, std::string _objectName);
};


class Cube : public PObject {
    public:
        Cube();
        BoundingBox boundingBox = { -1.0f, 1.0f,-1.0f, 1.0f, -1.0f, 1.0f};
        std::string cubeString = "CUBECUBE! \n";

};


#endif 