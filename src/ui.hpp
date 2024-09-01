
#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <vector>

#include "shader.hpp"
#include "uiElement.hpp"
#include "Types.hpp"




class UI {
    public:

        unsigned int uiVAO = 0;
        unsigned int uiVBO = 0;


        unsigned int squareVAO = 0;
        unsigned int squareVBO = 0;

        int windowWidth = 0;
        int windowHeight = 0;


        std::vector<UiElement> uiElements;
        std::vector<float> uiVertices;

        float uiTransform16[16] = {
                                        2.0f / 800, 0, 0, -1.0f,
                                        0, 2.0f / 600, 0, -1.0f,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1, };
        

        unsigned int uiTransformLoc;

        void createUiRectange(int height, int width, int x, int y, Vec3 color);


        UI() {};

        void reloadUi();

        void setWindowSize(int width, int height);

        void newShaderPlease(const char* vertexPath, const char* fragmentPath);

        void loadUiFile(const char* uiFilePath);

        void renderUI();

        

        Shader shader;

};

void UI::reloadUi(){

    uiElements.clear();

    uiTransform16[0] = 2.0f / windowWidth;
    uiTransform16[5] = 2.0f / windowHeight;

    createUiRectange(100, 100, 700, 500, white);
    createUiRectange(100, 100, 700, 400, black);
    createUiRectange(100, 100, 700, 300, red);
    createUiRectange(100, 100, 700, 200, green);
    createUiRectange(100, 100, 700, 100, blue);

    loadUiFile("src/main.psoui");

}

void UI::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}

// void UI::updateUiOnNewWindowSize(int width, int height){
    
// }


void UI::loadUiFile(const char* uiFilePath){
    // std::cout << std::endl << "Loading psoui: " << uiFilePath << std::endl;
    

    std::ifstream psouiFile;
    std::stringstream psouiStream;
    std::string psouiString;

    psouiFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {

        psouiFile.open(uiFilePath);


        psouiStream << psouiFile.rdbuf();

        psouiString = psouiStream.str();

        psouiFile.close();

        std::string keyValString;
        std::string key;
        std::string value;
        std::string line;
        // float number;


        // UiElement uiElem_;

        // Grab line
        while (std::getline(psouiStream, line)) {

            std::istringstream lineStream (line);    

            UiElement uiElem_;

            // split line into key-value pairs
            while (std::getline(lineStream, keyValString, ';')) {

                // for(std::string KV : item){}
                // std::cout << keyValString << std::endl;

                std::string::size_type n;
                n = keyValString.find(":");

                // error parsing key-value pair
                if (n == std::string::npos) // -1 ??
                    std::cout << "ERROR::PARSING_PSOUI_FILE. "  << __FILE__ << std::endl;
                

                // keyValString.copy(key, n);
                key = keyValString.substr(0, n);
                value = keyValString.substr(n + 1, keyValString.size());
                // std::cout << key << std::endl;
                // std::cout << value << std::endl;

                

                if(key == "name"){
                    uiElem_.name = value;
                }
                else if (key == "type") {
                    uiElem_.type = value;
                }
                else if (key == "x") {
                    if (value[0] == 'L')
                        uiElem_.x = std::stoi(value.substr(1, value.size()));
                    else if (value[0] == 'R')
                        uiElem_.x = windowWidth - std::stoi(value.substr(1, value.size()));
                }
                else if (key == "y") {
                    if (value[0] == 'B')
                        uiElem_.y = std::stoi(value.substr(1, value.size()));
                    else if (value[0] == 'T')
                        uiElem_.y = windowHeight - std::stoi(value.substr(1, value.size()));
                }
                else if (key == "w") {
                    uiElem_.width = std::stoi(value);
                }
                else if (key == "h") {
                    uiElem_.height = std::stoi(value);
                }
                else if (key == "c") {
                    
                    if(value == "black"){
                        uiElem_.color = black;
                    }
                    else if (value == "white") {
                        uiElem_.color = white;
                    }
                    else if (value == "red") {
                        uiElem_.color = red;
                    }
                    else if (value == "green") {
                        uiElem_.color = green;
                    }
                    else if (value == "blue") {
                        uiElem_.color = blue;
                    }
                }


            }

            if(uiElem_.type == "rectangle"){
                createUiRectange(uiElem_.height, uiElem_.width, uiElem_.x, uiElem_.y, uiElem_.color);
            }

            // uiElements.push_back(uiElem_);

            // for(std::string KV : item){}
            // std::cout << item << std::endl;

            // std::stringstream itemStream(item);
            // itemStream >> number;

            // // Add the parsed float to the vector
            // this->vertices.push_back(number);
        }

        std::cout << "PSUI OK. " << uiFilePath << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << " ERROR. " << uiFilePath << " : tried loading psoui file. " << std::endl;
        std::cout << "ERROR::READING_PSOUI_FILE" << e.what() << std::endl;
    }


}

void UI::createUiRectange(int height, int width, int x, int y, Vec3 color) {
    UiElement uiElem_;
    for(int i = 0; i<uiElem_.vertexCount*6; i += 6){
        uiElem_.squareVertices[i] = uiElem_.squareVertices[i] * width + x;
        uiElem_.squareVertices[i+1] = uiElem_.squareVertices[i+1] * height + y;

        uiElem_.squareVertices[i + 3] = color.x;
        uiElem_.squareVertices[i + 4] = color.y;
        uiElem_.squareVertices[i + 5] = color.z;
    }

    uiElem_.activated = 1;

    // uiElem_.vertices
    uiElements.push_back(uiElem_);
}





void UI::newShaderPlease(const char* vertexPath, const char* fragmentPath){
    shader.buildShaderProgram(vertexPath, fragmentPath);
}

void UI::renderUI()
{
    if (uiVAO == 0)
    {

        glGenVertexArrays(1, &uiVAO);
        glGenBuffers(1, &uiVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uiElements.size() * 36, uiVertices.data(), GL_STATIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(float), vertexVector.data(), GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(uiVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


    }

    

    // Update the VAO with active elements
    uiVertices.erase(uiVertices.begin(), uiVertices.end());
    
    int elemCount = 0;
    for(UiElement uiElem_ : uiElements){
        if(uiElem_.activated){
            elemCount++;
            for(float fl_ : uiElem_.squareVertices){
                uiVertices.push_back(fl_);
            }
        }
    }

    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uiElements.size() * 36, uiVertices.data(), GL_STATIC_DRAW);



    shader.setMat4("uiTransform", uiTransform16);
    // shader.setBool("isSquare", 1);

    // std::cout << "sizeof(float) * uiElements.size() * 36 == " << sizeof(float) * uiElements.size() * 36 << std::endl;
    // std::cout << "uiVertices.data() == " << uiVertices.data() << std::endl;

    glDrawArrays(GL_TRIANGLES, 0, uiElements.size()*6);

    // std::cout << "elemCount == " << elemCount << std::endl;
    // std::cout << "uiVertices.size() == " << uiVertices.size() << std::endl;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float playVertices[] = {
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



#endif