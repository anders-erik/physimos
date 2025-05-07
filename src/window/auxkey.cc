
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "auxwin.hh"

namespace window {


Key get_key_from_glfw_key(int _glfw_key){
    Key key;
    switch (_glfw_key){

    case GLFW_KEY_ESCAPE:   key = Key::Esc;       break;
    case GLFW_KEY_P:        key = Key::p;       break;
    
    default:
        break;
    }

    return key;

}

}