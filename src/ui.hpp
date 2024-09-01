
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
        unsigned int uiIsCharVAO = 0;
        unsigned int uiIsCharVBO = 0;


        // unsigned int squareVAO = 0;
        // unsigned int squareVBO = 0;

        int windowWidth = 0;
        int windowHeight = 0;

        // CHAR TEXTURING
        unsigned int charTexture;

        std::vector<float> isCharBuffer;

        void setCharacterTextureData(std::vector<unsigned char> data, unsigned int imgWidth, unsigned int imgHeight);
        void createUiChar(int fontHeight, int x, int y, char chValue, Vec3 color);
        void addStringUi(int fontHeight, int x, int y, std::string str);
        std::vector<unsigned char> charImageBuffer;
        unsigned int charImgWidth = 0;
        unsigned int charImgHeight = 0;



        std::vector<UiElement> uiElements;
        std::vector<float> uiVertexFloatBuffer;

        float uiTransform16[16] = {
                                        2.0f / 800, 0, 0, -1.0f,
                                        0, 2.0f / 600, 0, -1.0f,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1, };
        

        unsigned int uiTransformLoc;

        void createUiRectange(int height, int width, int x, int y, Vec3 color);
        


        UI() {};

        void init();

        void reloadUi();

        void setWindowSize(int width, int height);

        void newShaderPlease(const char* vertexPath, const char* fragmentPath);

        void loadUiFile(const char* uiFilePath);

        void renderUI();

        

        Shader shader;

};



void UI::init() {

    glGenVertexArrays(1, &uiVAO);
    glGenBuffers(1, &uiVBO);
    
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uiElements.size() * 36, uiVertexFloatBuffer.data(), GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(float), vertexVector.data(), GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(uiVAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);



    // CHAR BUFER
    // glGenVertexArrays(1, &uiIsCharVAO);
    glGenBuffers(1, &uiIsCharVBO);
    glBindBuffer(GL_ARRAY_BUFFER, uiIsCharVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * uiElements.size(), {}, GL_STATIC_DRAW);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);


    // TEXTURE
    glGenTextures(1, &charTexture);
    glBindTexture(GL_TEXTURE_2D, charTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, charImgWidth, charImgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, charImageBuffer.data());
    // std::cout << "charImageBuffer.size(): " << charImageBuffer.size() << std::endl;
    glGenerateMipmap(GL_TEXTURE_2D);

    // glUniform1i(glGetUniformLocation(shader.ID, "charTexture"), 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void UI::setCharacterTextureData(std::vector<unsigned char> data, unsigned int imgWidth, unsigned int imgHeight) {
    charImageBuffer = data;
    charImgWidth = imgWidth;
    charImgHeight = imgHeight;
}


void UI::newShaderPlease(const char* vertexPath, const char* fragmentPath) {
    shader.buildShaderProgram(vertexPath, fragmentPath);
}


void UI::reloadUi(){

    uiElements.clear();

    uiTransform16[0] = 2.0f / windowWidth;
    uiTransform16[5] = 2.0f / windowHeight;

    createUiRectange(100, 100, 700, 500, white);
    createUiRectange(100, 100, 700, 400, black);
    createUiRectange(100, 100, 700, 300, red);
    createUiRectange(100, 100, 700, 200, green);
    createUiRectange(100, 100, 700, 100, blue);

    // createUiChar(300, 0, 120, 'A', white);
    addStringUi(20, 200, 30, "A!!!123000");
    addStringUi(20, 200, 0, "Second string! []{}()");

    loadUiFile("src/main.psoui");
}



void UI::renderUI()
{
    glUseProgram(shader.ID);

    isCharBuffer.clear();

    // Update the VAO with active elements
    uiVertexFloatBuffer.erase(uiVertexFloatBuffer.begin(), uiVertexFloatBuffer.end());

    int elemCount = 0;
    for (UiElement uiElem_ : uiElements) {
        if (uiElem_.activated) {
            elemCount++;

            // ONE FOR EACH VERTEX
            for (size_t i = 0; i < 6; i++)
            {
                if (uiElem_.isChar > 1.0)
                    isCharBuffer.push_back(1.0);
                else
                    isCharBuffer.push_back(0.0);
            }
            
            
            // THESE ARE NOT VERTICES!!!!!!!!!!
            for (float fl_ : uiElem_.vertexFloatBuffer) {
                

                uiVertexFloatBuffer.push_back(fl_);
            }
        }
    }
    // std::cout << "uiElements.size(): " << uiElements.size() << std::endl;
    // std::cout << "uiElements: " << uiElements << std::endl;
    
    
    
    
    // glUniform1i(glGetUniformLocation(shader.ID, "charTexture"), 0);
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, charTexture);

    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uiElements.size() * 48, uiVertexFloatBuffer.data(), GL_STATIC_DRAW);

    
    glBindBuffer(GL_ARRAY_BUFFER, uiIsCharVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * isCharBuffer.size(), isCharBuffer.data(), GL_STATIC_DRAW);
    



    shader.setMat4("uiTransform", uiTransform16);
    // shader.setBool("isSquare", 1);

    // std::cout << "sizeof(float) * uiElements.size() * 36 == " << sizeof(float) * uiElements.size() * 36 << std::endl;
    // std::cout << "uiVertexFloatBuffer.data() == " << uiVertexFloatBuffer.data() << std::endl;

    glDrawArrays(GL_TRIANGLES, 0, uiElements.size() * 6);
    

    // std::cout << "elemCount == " << elemCount << std::endl;
    // std::cout << "uiVertexFloatBuffer.size() == " << uiVertexFloatBuffer.size() << std::endl;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UI::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}




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
            // this->vertexFloatBuffer.push_back(number);
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
    for(int i = 0; i<uiElem_.vertexCount*8; i += 8){
        uiElem_.vertexFloatBuffer[i] = uiElem_.squareVertices[i] * width + x;
        uiElem_.vertexFloatBuffer[i + 1] = uiElem_.squareVertices[i + 1] * height + y;

        uiElem_.vertexFloatBuffer[i + 3] = color.x;
        uiElem_.vertexFloatBuffer[i + 4] = color.y;
        uiElem_.vertexFloatBuffer[i + 5] = color.z;

        uiElem_.vertexFloatBuffer[i + 6] = uiElem_.charVertices[i + 6];
        uiElem_.vertexFloatBuffer[i + 7] = uiElem_.charVertices[i + 7];
    }

    uiElem_.activated = 1;

    // uiElem_.vertices
    uiElements.push_back(uiElem_);
}


void UI::addStringUi(int fontHeight, int x, int y, std::string str) {
    int charCount = 0;
    int fontWidth = fontHeight/1.20;
    for(char ch : str){
        createUiChar(fontHeight, x+charCount*fontWidth, y, ch, white);
        charCount++;
    }
}


void UI::createUiChar(int fontHeight, int x, int y, char chValue, Vec3 color) {
    UiElement uiElem_;
    for (int i = 0; i < uiElem_.vertexCount * 8; i += 8) {
        uiElem_.vertexFloatBuffer[i] = uiElem_.charVertices[i] * fontHeight + x;
        uiElem_.vertexFloatBuffer[i + 1] = uiElem_.charVertices[i + 1] * fontHeight + y;

        uiElem_.vertexFloatBuffer[i + 3] = color.x;
        uiElem_.vertexFloatBuffer[i + 4] = color.y;
        uiElem_.vertexFloatBuffer[i + 5] = color.z;

        // Determine UV coordinates for each vertex
        int column = chValue % 10;
        int row = (chValue / 10) - 3;

        uiElem_.vertexFloatBuffer[i + 6] = uiElem_.charVertices[i + 6] * 0.09 + (float)column * 0.1;
        uiElem_.vertexFloatBuffer[i + 7] = uiElem_.charVertices[i + 7] * 0.1 + 0.9 - (float)row*0.1;
    }

    uiElem_.activated = 1;
    uiElem_.isChar = 10.0;

    // uiElem_.vertexFloatBuffer
    uiElements.push_back(uiElem_);
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