#ifndef WO_RENDERER_HPP
#define WO_RENDERER_HPP

#include <vector>
#include <string>

// #include "WorldObject.hpp"
#include "shader.hpp"
#include "render_types.hh"
// #include "Model.hpp"
// #include "simulator.hpp"

namespace render {


class Renderer {
    public:
        Renderer() {}; // getting called on each world object ??
        Renderer(RENDPIPE rendpipe, std::string modelname); // NEW PIPLINE CONSTRUCTOR

        // void createRenderer(int worldObjectType);
        // void createSimulatorRenderer();

        Shader* shader;
        void setShaderProgram(Shader* shader);
        unsigned int vao;
        unsigned int vbo;
        void setVaoVbo330();
        void setVaoVbo332();
        // It was not being used, So I disabled it for the sake of making refactoring easier [2024-09-27]
        // void setVaoVbo_obj(Model _model);
        
        void createSimulatorRenderer(std::vector<float> _vertices);

        


};


}


#endif