#ifndef UI_PRIMITIVE_HH
#define UI_PRIMITIVE_HH

#include <string>

#include "render/shader.hpp"

#include "ui/ui_globals.hh"
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


typedef struct Transform {
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

    /// x window coordinate of primitive's bottom left corner, relative to bottom left of window.
    int x_real = 0;
    /// y window coordinate of primitive's bottom left corner, relative to bottom left of window.
    int y_real = 0;

    // Transformation matrix for shading
    float uiPrimitiveTransform16[16] = {    1, 0, 0, 0,
                                            0, 1, 0, 0,
                                            0, 0, 1, 0,
                                            0, 0, 0, 1, };

} Transform;



class Primitive {
    public:
        Primitive() {};

        std::string id = "";
        void printId();


        // Primitive Tree
        Primitive* parent = nullptr;
        std::vector<Primitive*> children;

        bool isLeaf();
        void appendChild(Primitive* childPrimitive);
        std::vector<Primitive*> flattenTree();
        // TODO: move the tmp flat vector into primitive
        void appendtoFlatTreeNested(Primitive* _primitive);

        

        // UI Transformation 
        UI::Transform uiTransform;
        void setX(int _x);
        void setY(int _y);
        void setHeight(int _height);
        void setWidth(int _width);
        // Set the transformation matrix for shading
        void reloadHWXY();
        bool containsPoint(double x, double y);
        bool childrenContainPoint(double _x, double _y);
        // Set the shader materices using the ui transform object
        void updateShaderMatrixesRecursively();



        // TEXT
        std::string text = "";
        FontSize fontSize = FontSize::f24;
        void setString(std::string _str);
        bool isTextPrimitive();



        // RENDERING
        Shader* shader = nullptr;
        unsigned int vao;
        unsigned int vbo; 
        /** The currently rendered texture for UI::Primitive. */
        unsigned int glTexture;
        unsigned int defaultTexture;
        unsigned int hoverTexture;
        unsigned int selectedTexture;
        void initGraphics();
        void generateTextures();
        void renderRecursive();


        // STATE & BEHAVIOR
        PrimitiveState state = PrimitiveState::Default;
        void setState(PrimitiveState _newState);
        bool isHoverable = false;
        bool isClickable = false;
        virtual UI::Action click();
        // void (*clickCallback)() = nullptr;



        // UI COMPONENT

        // Primitives are almost always part of a component. 
        // Components usually bind a specific object and are then interacted with using its context.
        // This enables primitive methods to access to the component which it belongs and its bound objects.
        void* componentContext = nullptr;        
};




}



#endif