#ifndef UI_PRIMITIVE_HH
#define UI_PRIMITIVE_HH

#include <string>

#include "render/shader.hpp"

namespace UI {

class Primitive; // forward declare


typedef enum PrimitiveType {
    Rectangle = 0,
} PrimitiveType;



struct PrimitiveInfo {
    PrimitiveType primitiveType;
    int height = 0;
    int width = 0;
    int x = 0;
    int y = 0;

    // Colors
    int R = 0;
    int G = 0;
    int B = 0;
    int A = 0;

    Primitive* parent = nullptr;
};



class Primitive {
    public:
        Primitive() {};
        Primitive(PrimitiveInfo* _primitiveInfo_ptr);

        // void setGlData();
        void generateTextures();

        void init();
        void update();
        void render();

        bool containsPoint(double x, double y);

        PrimitiveType primitiveType;

        int height = 0;
        int width = 0;
        int x = 0;
        int y = 0;

        // Colors
        int R = 0;
        int G = 0;
        int B = 0;
        int A = 0;

        Primitive* parent = nullptr;



        PrimitiveInfo* primitiveInfo_ptr;


        Shader* shader = nullptr;

        unsigned int vao;
        unsigned int vbo; 

        /** The currently rendered texture for UI::Primitive. */
        unsigned int glTexture;

        // Available textures for different ui pritimitve states
        unsigned int defaultTexture;
        unsigned int hoverTexture;
        unsigned int selectedTexture;


        float uiPrimitiveTransform16[16] = {
                                    1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1,
        };

        float squareVertices[30] = {
           0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
           1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
           0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // top-left
           1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
           0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
           1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        };
        
};




}



#endif