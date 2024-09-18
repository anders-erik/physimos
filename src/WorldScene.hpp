
#include "WorldObject.hpp"

void ws_init();

void ws_createWorldObjects();
void ws_resetWorldObjects();

void ws_loadTextures();

void ws_physics();
int worldObjectCollidingWithGround_aabb_z(WorldObject& ground, WorldObject& wo2);

void ws_update();