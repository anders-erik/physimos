
#pragma once

#include <vector>
#include "math/vecmat.hh"

namespace opengl {


typedef struct RendererVector {
    unsigned int program;
    unsigned int VAO;
    unsigned int VBO;
    
    // std::vector<AxesRenderVertex> vertices;
    // Basis basis;

    std::vector<f3> arrow;

    float scale = 4.0f;
    f3 color = f3(1.0f, 1.0f, 1.0f);

    void set_project_view_matrix(m4f4 project_mat, m4f4 view_mat);
    void render(f3 vector, f3 translation);
    void init();
} RendererVector;


}

