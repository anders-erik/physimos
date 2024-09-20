
#include "wo_renderer.hpp"

#include <vector>


extern Shader wireframeShader;



void Renderer::createSimulatorRenderer(std::vector<float> _vertices) {
    std::cout << "INIT SIMULATOR RENDERER" << std::endl;

    // shader->buildShaderProgram()
    shader = &wireframeShader;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices[0]) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);
    // printf("triangel : %lu\n", sizeof(triangle));
    // printf("data()   : %lu\n", sizeof(worldCube1.vertices.data()); // I guess this returns the size of the pointer to the underlaying data/array on the heap?
    // printf("data()   : %lu\n", sizeof(worldCube1.vertices[0])*worldCube1.vertices.size());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
}

void Renderer::setShaderProgram(Shader* shader) {
    this->shader = shader;
}

void Renderer::setVaoVbo330() {
    
}
void Renderer::setVaoVbo332() {
   
}

void Renderer::setVaoVbo_obj(Model _model) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    // REPLACE VERTEX NORMALS WITH COLORS!
    for (unsigned int i = 0; i < _model.vertices.size(); i += 8) {
        // vertices[i + 5] = 0.0f;
        // vertices[i + 6] = 0.0f;
        // vertices[i + 7] = 0.0f;

        // vertices[i + 5] = Kd[0];
        // vertices[i + 6] = Kd[1];
        // vertices[i + 7] = Kd[2];

        // model.vertices[i + 5] = model.Kd[0];
        // model.vertices[i + 6] = model.Kd[1];
        // model.vertices[i + 7] = model.Kd[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_model.vertices[0]) * _model.vertices.size(), _model.vertices.data(), GL_STATIC_DRAW);
    // printf("triangel : %lu\n", sizeof(triangle));
    // printf("data()   : %lu\n", sizeof(worldCube1.vertices.data()); // I guess this returns the size of the pointer to the underlaying data/array on the heap?
    // printf("data()   : %lu\n", sizeof(worldCube1.vertices[0])*worldCube1.vertices.size());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);

}