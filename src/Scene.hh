#ifndef SCENE_HH
#define SCENE_HH

#include <iostream>

#include "Camera.hpp"

class Scene {
public:
    Scene() { std::cout << "NEW SCENE" << std::endl;};

    Camera* camera;

    
    // unsigned int _viewObjLoc;
    // unsigned int _sanityObjLoc;
    // unsigned int _perspectiveObjLoc;
};


#endif