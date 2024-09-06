#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include <vector>

#include "WorldObject.hpp"



void wr_init();

void wr_render(std::vector<WorldObject>& _worldObjects);

void update_sim_timestep();



void modelFunction();





#endif