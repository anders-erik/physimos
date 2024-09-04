#ifndef WORLD_RENDERER_HPP
#define WORLD_RENDERER_HPP

#include <vector>

#include "WorldObject.hpp"

void modelFunction();

class WorldRenderer {
    public:
        WorldRenderer() {};
        
        std::vector<WorldObject> worldObjects;

        void newWorldObject(char * modelPath);
};

#endif