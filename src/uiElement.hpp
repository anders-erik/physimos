#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <vector>
#include <string>

#include "Types.hpp"


typedef enum ElementType {
    RECTANGLE = 0,
    STRING = 1,
    TEXTURE = 2,
} ElementType;




class UiElement {
    public:
        UiElement() {};
        UiElement(std::string _name) {name = _name;};

        // REFACTOR - 2024-10-04
        UiElement* parent = nullptr;
        std::vector<UiElement*> children;
        
        void render();


    
        int vertexCount = 6;
        // float vertexFloatBuffer[6*6];
        // float color[3] = {1.0f, 1.0f, 1.0f};
        Vec3 color = { 1.0f, 1.0f, 1.0f };
        int activated = 0;
        std::string name;
        std::string type;
        ElementType elementType;

        // Texture / Background
        int hasTexture = 0;
        unsigned int glTexture;
        std::vector<unsigned char> elementTextureBuffer;
        unsigned int textureWidth;
        unsigned int textureHeight;
        std::string elementTexturePath;

        // Element Text/string
        float isChar = 0.0;
        bool isStringElement = false;
        std::vector<UiElement> stringElements;
        char charValue = 0;


        int height;
        int width;
        int x;
        int y;
        std::vector<float> vertexFloatBuffer = {
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // top-right
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // top-left
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // top-right
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        };
        std::vector<float> squareVertices = {
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // top-right
            0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // top-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // top-right
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        };
        std::vector<float> charVertices = {
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            1.0f, 1.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right
            0.0f, 1.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // top-left
            1.0f, 1.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        };


};

class TextElement : UiElement {
    public:
        std::string text = "4";
};


#endif