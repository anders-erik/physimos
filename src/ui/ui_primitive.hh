#ifndef UI_PRIMITIVE_HH
#define UI_PRIMITIVE_HH

#include <string>

#include "render/shader.hpp"

#include "ui/font.hh"

namespace UI {

class Primitive; // forward declare


enum PrimitiveState {
    Default = 1,
    Hover = 2,
    Selected = 3,
};

enum Unit {
    Pixel = 0,
    Percent = 1
};

enum VertRef {
    // Distance will be measured from parent top to primitive top
    Top = 0,
    // Distance will be measured from parent bottom to primitive bottom
    Bottom = 1
};

enum HoriRef {
    // Distance will be measured from parent left edge to primitive left edge
    Left = 0,
    // Distance will be measured from parent right edge to primitive right edge
    Right = 1
};

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
        void reloadHWXY();
        void initGraphics();
        void generateTextures();

        void init();
        void update();
        void render();

        bool childrenContainPoint(double _x, double _y);
        bool containsPoint(double x, double y);

        std::string id = "";
        void printId();

        bool isLeaf();

        PrimitiveType primitiveType = PrimitiveType::Rectangle;

        size_t height = 200;
        Unit h_unit = Unit::Pixel;
        size_t width = 200;
        Unit w_unit = Unit::Pixel;

        // Raw input values to setX/y methods
        int x_input = 0;
        int y_input = 0;
        // Units of the x/y input values
        Unit x_unit = Unit::Pixel;
        Unit y_unit = Unit::Pixel;
        // x/y is measured from where?
        HoriRef horiRef = HoriRef::Left;
        VertRef vertRef = VertRef::Bottom;
        // Getters/setters
        void setX(int _x);
        void setY(int _y);
        void setHeight(int _height);
        void setWidth(int _width);


        /// x window coordinate of primitive's bottom left corner, relative to bottom left of window.
        int x_real = 0;
        /// y window coordinate of primitive's bottom left corner, relative to bottom left of window.
        int y_real = 0;


        // Colors
        int R = 155;
        int G = 155;
        int B = 155;
        int A = 255;


        std::string text = "";
        FontSize fontSize = FontSize::f24;
        void setString(std::string _str);
        bool isTextPrimitive();


        Primitive* parent = nullptr;

        std::vector<Primitive*> children;
        void appendChild(Primitive* childPrimitive);

        std::vector<Primitive*> flattenTree();
        void appendtoFlatTreeNested(Primitive* _primitive);

        PrimitiveInfo* primitiveInfo_ptr;


        Shader* shader = nullptr;

        unsigned int vao;
        unsigned int vbo; 

        /** The currently rendered texture for UI::Primitive. */
        unsigned int glTexture;


        // Available textures for different ui pritimitve states
        PrimitiveState state = PrimitiveState::Default;
        void setState(PrimitiveState _newState);
        bool isHoverable = false;
        bool isClickable = false;
        void click();
        // void (*clickCallback)() = nullptr;
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