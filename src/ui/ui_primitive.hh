#ifndef UI_PRIMITIVE_HH
#define UI_PRIMITIVE_HH

#include <string>

#include "render/shader.hpp"

#include "ui/ui_globals.hh"
#include "ui/font.hh"

namespace UI {

class Primitive; // forward declare

typedef struct Color {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
} Color;


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


typedef struct PositionInput {
    bool hasBeenChangedFlag = false;

    // Raw input values to setX/y methods
    int x_input = 0;
    int y_input = 0;
    // Units of the x/y input values
    Unit x_unit = Unit::Pixel;
    Unit y_unit = Unit::Pixel;
    // x/y is measured from where?
    HoriRef horiRef = HoriRef::Left;
    VertRef vertRef = VertRef::Bottom;

    // Setters that will set the 'hasBeenChangedFlag'. Flag will be read during update calls before (or from) render function
    void setx(int x);
    void sety(int y);
    void setxunit(UI::Unit x_unit);
    void setyunit(UI::Unit y_unit);
    void setxref(UI::HoriRef horiRef);
    void setyref(UI::VertRef vertiRef);

} PositionInput;

typedef struct PositionReal {
    /// x window coordinate of primitive's bottom left corner, relative to bottom left of window.
    int x_real = 0;
    /// y window coordinate of primitive's bottom left corner, relative to bottom left of window.
    int y_real = 0;

} PositionReal;

// typedef struct PositionUi {
//     PositionInput posInput;
//     PositionReal  posReal;

//     void setx(std::string x_str);
//     void sety(std::string x_str);
// } PositionUi;


typedef struct Transform {
    size_t height = 200;
    Unit h_unit = Unit::Pixel;
    size_t width = 200;
    Unit w_unit = Unit::Pixel;

    // Input string
    std::string input_string = "";
    // Raw input value parsed from string
    int x_input = 0;
    int y_input = 0;
    // 2025-02-17/18
    // input values converted to pixels
    bool hasBeenChangedFlag = false;
    int x_input_px = 0;
    int y_input_px = 0;
    // int x_input_percent_window = 0;
    // int y_input_percent_window = 0;

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
        void setXrecursive(int x_input);
        void setYrecursive(int y_input);
        void setHeight(int _height);
        void setWidth(int _width);

        // Updates: 2025-02-17
        void set_x(std::string x_str);
        void set_y(std::string y_str);
        void update_x_real_recursive();
        void update_y_real_recursive();
        // UI::PositionReal posReal;
        // UI::PositionInput posInput;
        // void updateXrealRecursively();
        // void updateYrealRecursively();
        // void setUiTransform(::UI::Transform _uiTransform);


        // Set the transformation matrix for shading
        void updateTransformationMatrix();
        bool containsPoint(double x, double y);
        bool childrenContainPoint(double _x, double _y);
        // Set the shader matrices using the ui transform object
        void updateShaderMatrixesRecursively();



        // TEXT
        std::string text = "";
        FontSize fontSize = FontSize::f24;
        void setString(std::string _str);
        bool isTextPrimitive();



        // RENDERING
        // Primitives without parents will have a z-value of 1. Each child will recieve a z value of parent.z + 1.
        // A z value of 0 will not be rendered ?
        int z = 1;
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
        void setDefaultColor(Color color);


        // STATE & BEHAVIOR
        PrimitiveState state = PrimitiveState::Default;
        void setState(PrimitiveState _newState);
        bool isHoverable = false;
        bool isClickable = false;
        virtual UI::Action click();
        // void (*clickCallback)() = nullptr;



        // UI COMPONENT

        // Primitives are almost always part of a component: a collection of primitives and a tracked object bound to it.
        // This pointer enables primitives to access the same scope and objects as the component to which it belongs. 
        void* component = nullptr;        
};




}



#endif