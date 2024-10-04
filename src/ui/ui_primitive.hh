#ifndef UI_PRIMITIVE_HH
#define UI_PRIMITIVE_HH

#include "render/shader.hpp"

namespace UI {


class UiPrimitive {
    public:
        UiPrimitive() {};

        Shader* shader = nullptr;

        unsigned int vao;
        unsigned int vbo; // 
        unsigned int glTexture;

        
};




}



#endif