#ifndef WORLD_SCENE_HPP
#define WORLD_SCENE_HPP

#include <vector>
#include <string>

#include "WorldObject.hpp"

#include "WorldSimulator.hh"

#include "Camera.hpp"

namespace PScene {

    extern WorldObject* bouncyTrianglePointer;
    extern WorldObject* groundObjectPointerScene1;


    class Scene {
    public:
        std::string name;
        std::vector<WorldObject*> pObjects;
        std::vector<WorldSimulator*> worldSimulators;
        ::PCamera::Camera* camera;

        Scene() : name("default_scene") {};
    };


    void init();
    void updateCurrentScene();
    void renderCurrentScene();

    Scene* getCurrentScene();

    void renderPObject(::WorldObject* pObject);

    float* _mul_vec3_mat16(float* _vec3, float* _mat16);
    void physics();
    int _worldObjectCollidingWithGround_aabb_z(WorldObject& ground, WorldObject& wo2);
    void populateScene1();

    void addPObjectToCurrentScene(::WorldObject* pObject);
    WorldObject* getWorldObjectByName(std::string _name);
    std::vector<WorldObject*> getPObjects();

}




#endif