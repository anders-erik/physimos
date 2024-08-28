
#include <iostream>
#include <vector>

#include "shader.hpp"

struct Square {
    int vertices = 6;
    int activated = 0;
    int height;
    int width;
    int x;
    int y;
    float squareTransform16f[16] = {
                                        1.0f / 800, 0, 0, -1.0f,
                                        0, 1.0f / 600, 0, -1.0f,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1, };
};


class UI {
    public:

        unsigned int uiVAO = 0;
        unsigned int uiVBO = 0;


        unsigned int squareVAO = 0;
        unsigned int squareVBO = 0;
        float squareTransform16f[16] = {
                                        1.0f/800, 0, 0, -1.0f,
                                        0, 1.0f /600, 0, -1.0f,
                                        0, 0, 1, 0, 
                                        0, 0, 0, 1, };

        unsigned int uiTransformLoc;
        struct Square square1;
        struct Square square2;
        void defineSquare1(int height, int width, int x, int y);
        void defineSquare2(int height, int width, int x, int y);
        float squareVertices[36] = {
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // top-right
            0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // top-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // top-right
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        };

        UI();

        void newShaderPlease(const char* vertexPath, const char* fragmentPath);

        void renderUI();

        

        Shader shader;

};


void UI::defineSquare1(int height, int width, int x, int y){
    square1 = {6, 1, height, width, x, y};
}
void UI::defineSquare2(int height, int width, int x, int y) {
    square2 = { 6, 1, height, width, x, y };
}


UI::UI(){
    std::cout << "New UI class! \n";
    // shader.buildShaderProgram("src/shaders/ui.vs", "src/shaders/ui.fs");
}

void UI::newShaderPlease(const char* vertexPath, const char* fragmentPath){
    shader.buildShaderProgram(vertexPath, fragmentPath);
}


void UI::renderUI()
{
    // initialize (if necessary)
    if (uiVAO == 0)
    {
        float vertices[] = {
            // back face
            0.7f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-left
            1.0f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f, // top-right
            1.0f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-right         
            1.0f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f, // top-right
            0.7f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-left
            0.7f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f,  // top-left

            // Play triangle
            0.8f, 0.9f, -1.0f,  0.0f,  1.0f, 0.0f, // top-left
            0.8f,  0.8f, -1.0f,  0.0f,  1.0f, 0.0f, // botton-left
            0.9f, 0.85f, -1.0f,  0.0f,  1.0f, 0.0f, // middle-right
        };


        glGenVertexArrays(1, &uiVAO);
        glGenBuffers(1, &uiVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(float), vertexVector.data(), GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(uiVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        


        std::vector<float> vertexVector = {};

        // SQUARE 1
        if(square1.activated){
            for (const float coord : squareVertices){
                // std::cout << coord << " " << std::endl;
                vertexVector.push_back(coord);
            }
            // std::cout << "SQUARE 1 ADDED! \n ";
            
            square1.squareTransform16f[0] = square1.squareTransform16f[0] * square1.width;
            square1.squareTransform16f[5] = square1.squareTransform16f[5] * square1.height;
            square1.squareTransform16f[3] += square1.x / 800.0f;
            square1.squareTransform16f[7] += square1.y / 600.0f;
        }
        // uiTransformLoc = glGetUniformLocation(shader.ID, "uiTransform");
        shader.setMat4("uiTransform", square1.squareTransform16f);
        shader.setBool("isSquare", 1);

        
        if (square2.activated) {
            for (const float coord : squareVertices) {
                // std::cout << coord << " " << std::endl;
                vertexVector.push_back(coord);
            }
            // std::cout << "SQUARE 2 ADDED! \n ";

            square2.squareTransform16f[0] = square2.squareTransform16f[0] * square2.width;
            square2.squareTransform16f[5] = square2.squareTransform16f[5] * square2.height;
            square2.squareTransform16f[3] += square2.x / 800.0f;
            square2.squareTransform16f[7] += square2.y / 600.0f;
        }
        // // uiTransformLoc = glGetUniformLocation(shader.ID, "uiTransform");
        // shader.setMat4("uiTransform", square2.squareTransform16f);
        // shader.setBool("isSquare", 1);

        glGenVertexArrays(1, &squareVAO);
        glGenBuffers(1, &squareVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
        glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(float), vertexVector.data(), GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(squareVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
    }
    // render Cube
    glBindVertexArray(uiVAO);
    shader.setBool("isSquare", 0);
    glDrawArrays(GL_TRIANGLES, 0, 9);

    glBindVertexArray(squareVAO);
    shader.setBool("isSquare", 1);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(0);
}


