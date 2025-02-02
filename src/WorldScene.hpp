#ifndef WORLD_SCENE_HPP
#define WORLD_SCENE_HPP

#include <vector>

#include "WorldObject.hpp"


namespace WS {

WorldObject* getWorldObjectByName(std::string _name);

}

void ws_init();

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