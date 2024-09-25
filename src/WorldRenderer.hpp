#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include <vector>

#include "WorldObject.hpp"
// #include "simulator.hpp"



void wr_init();

void wr_render(std::vector<WorldObject>& _worldObjects);

void wr_renderWorldShader(WorldObject& _worldObject);
void wr_renderWorldObjShader(WorldObject& _worldObject);
void wr_renderWireframeShader(WorldObject& _worldObject);
void wr_renderSimContainer(WorldObject& _worldObject);

// REMOVE?
void update_sim_timestep();






#endif