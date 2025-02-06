#ifndef WORLD_SCENE_HPP
#define WORLD_SCENE_HPP

#include <vector>

#include "WorldObject.hpp"

#include "WorldSimulator.hh"

#include "Camera.hpp"

namespace PScene {

    extern WorldObject* bouncyTrianglePointer;
    extern WorldObject* groundObjectPointerScene1;


    class Scene {
    public:
        Scene() {};

        std::vector<WorldObject*> pObjects;

        std::vector<WorldSimulator*> worldSimulators;

        Camera* camera;
        ::PCamera::Camera* pCamera;

    };


    void init();
    void updateCurrentScene();
    void renderCurrentScene();

    void renderPObject(::WorldObject* pObject);

    float* _mul_vec3_mat16(float* _vec3, float* _mat16);
    void physics();
    int _worldObjectCollidingWithGround_aabb_z(WorldObject& ground, WorldObject& wo2);
    void populateScene1();

    void addPObjectToCurrentScene(::WorldObject* pObject);
    WorldObject* getWorldObjectByName(std::string _name);
    std::vector<WorldObject*> getPObjects();

}


void ws_createSimulators();

void ws_createWorldObjects();
void ws_resetWorldObjects();

std::vector<WorldObject*> ws_getWorldObjects();

void ws_loadTextures();

float* mul_vec3_mat16(float* _vec3, float* _mat16);
void ws_physics();
int worldObjectCollidingWithGround_aabb_z(WorldObject& ground, WorldObject& wo2);

void ws_update();

void ws_update_objects();
void ws_render();



#endif