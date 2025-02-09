#ifndef WORLD_SCENE_HPP
#define WORLD_SCENE_HPP

#include <vector>
#include <string>

#include "pobject.hh"

#include "scene/WorldSimulator.hh"

#include "Camera.hpp"

namespace PScene {

    extern PObject* bouncyTrianglePointer;
    extern PObject* groundObjectPointerScene1;


    class Scene {
    public:
        std::string name;
        std::vector<PObject*> pObjects;
        std::vector<WorldSimulator*> worldSimulators;
        ::PCamera::Camera* camera;

        Scene() : name("default_scene") {};
    };


    void init();
    void updateCurrentScene();
    void renderCurrentScene();

    Scene* getCurrentScene();

    void renderPObject(::PObject* pObject);

    float* _mul_vec3_mat16(float* _vec3, float* _mat16);
    void physics();
    int _worldObjectCollidingWithGround_aabb_z(PObject& ground, PObject& wo2);
    void populateScene1();

    void addPObjectToCurrentScene(::PObject* pObject);
    PObject* getWorldObjectByName(std::string _name);
    std::vector<PObject*> getPObjects();

}




#endif