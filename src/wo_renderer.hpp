#ifndef WO_RENDERER_HPP
#define WO_RENDERER_HPP

#include <vector>

// #include "WorldObject.hpp"
#include "shader.hpp"
#include "Model.hpp"
// #include "simulator.hpp"


class Renderer {
    public:
        Renderer() {};
        // void createRenderer(int worldObjectType);
        // void createSimulatorRenderer();

        Shader* shader;
        void setShaderProgram(Shader* shader);
        unsigned int vao;
        unsigned int vbo;
        void setVaoVbo330();
        void setVaoVbo332();
        void setVaoVbo_obj(Model _model);
        
        void createSimulatorRenderer(std::vector<float> _vertices);


};


#endif