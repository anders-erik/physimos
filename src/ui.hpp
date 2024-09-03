
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
        void createUiChar(int fontHeight, int x, int y, char chValue, Vec3 color, UiElement &_uiStringElem);
        void updateStringUi(int fontHeight, int x, int y, std::string str, UiElement &_uiStringElem);
        std::vector<unsigned char> charImageBuffer;
        unsigned int charImgWidth = 0;
        unsigned int charImgHeight = 0;

        // String elements
        std::vector<UiElement> fpsStringElements;
        void updateFpsElement(int fps);

        


        std::vector<UiElement> uiElements;
        std::vector<float> uiVertexFloatBuffer;

        float uiTransform16[16] = {
                                        2.0f / 800, 0, 0, -1.0f,
                                        0, 2.0f / 600, 0, -1.0f,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1, };
        

        unsigned int uiTransformLoc;

        void createUiRectange(int height, int width, int x, int y, Vec3 color, std::string elemName);
        


        UI() {};

        void init();

        void reloadUi();

        void setWindowSize(int width, int height);

        void newShaderPlease(const char* vertexPath, const char* fragmentPath);

        void loadUiFile(const char* uiFilePath);

        void renderUI();

        

        Shader shader;

};




#endif