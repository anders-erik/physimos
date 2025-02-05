#ifndef PSCENE_HH
#define PSCENE_HH

#include <iostream>

#include "Camera.hpp"

class Scene {
public:
    Scene() { /* std::cout << "NEW SCENE" << std::endl; */};

    Camera* camera;

};


#endif