#ifndef SCENE_HH
#define SCENE_HH

#include <iostream>

#include "Camera.hpp"

class Scene {
public:
    Scene() { std::cout << "NEW SCENE" << std::endl;};

    Camera* camera;

};


#endif