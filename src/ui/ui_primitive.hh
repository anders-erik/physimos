#ifndef UI_PRIMITIVE_HH
#define UI_PRIMITIVE_HH

#include <string>

#include "render/shader.hpp"

#include "ui/ui_shader_texture.hh"
#include "ui/ui_shader_color.hh"

#include "ui/ui_globals.hh"
#include "ui/font.hh"

namespace UI {

class Primitive; // forward declare


typedef enum class PrimitiveType {
    Base,
    String,
    Component
} PrimitiveType;

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


/** Specifies the full transform state of UI primitive: size, position, render transform, size change flags.  */
typedef struct Transform {
    /** Intially set to true for full update during first render. */
    bool size_has_been_changed = true;
    std::string h_input_string = "";
    std::string w_input_string = "";
    size_t h_input = 100;
    size_t w_input = 100;
    Unit h_unit = Unit::Percent;
    Unit w_unit = Unit::Percent;
    size_t h_real = 0;
    size_t w_real = 0;



    // Input string
    std::string x_input_string = "";
    // Raw input value parsed from string
    int x_input = 0;
    int y_input = 0;
    // 2025-02-17/18
    // input values converted to pixels
    bool x_has_been_changed = true;
    bool y_has_been_changed = true;
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
        /** Initializes a new primitive ui object. Sets shader and initial textures. */
        Primitive();

        std::string id = "";

        PrimitiveType primitiveType = PrimitiveType::Base;

        // Primitive Tree
        Primitive* parent = nullptr;
        std::vector<Primitive*> children;
        


        // SIZE & POSITION

        UI::Transform uiTransform;

        /** Update h_input, h_unit, and set changed flag */
        void set_h(std::string h_str);
        /** Update w_input, w_unit, and set changed flag */
        void set_w(std::string w_str);
        void set_x(std::string x_str);
        void set_y(std::string y_str);

        /** Update the real height of primitive and its descendants, then reaset the change detection flag. */
        void update_h_real_recursive();
        /** Update the real width of primitive and its descendants, then reaset the change detection flag. */
        void update_w_real_recursive();
        void update_x_real_recursive();
        void update_y_real_recursive();

        bool containsPoint(double x, double y);



        // RENDERING

        ::UI::shader::TextureShader*    texture_shader;
        ::UI::shader::ColorShader*      color_shader;

        /** Set the 4x4 transformation matrix that will be set as shader uniform */
        void updateTransformationMatrix();
        
        /** Render current state of primitive */
        void render();
        /** Render current state of primitive and making this same call to all its children */
        void render_recursive();
        // Primitives without parents will have a z-value of 1. Each child will recieve a z value of parent.z + 1.
        // A z value of 0 will not be rendered ?
        int z = 1;



        bool has_texture = false;
        /** Set the rendered texture. */
        void set_texture(unsigned int glTextureName);

        /** The currently rendered texture for UI::Primitive. */
        unsigned int renderedTexture;
        /** Texture rendered when default state is set. */
        unsigned int defaultTexture;
        /** Texture rendered when hover state is set. */
        unsigned int hoverTexture;
        /** Texture rendered when selected state is set. */
        unsigned int selectedTexture;
        /** Generated when setting a new primitive string in PrimitiveString. */
        unsigned int privateStringTexture;
        
        // bool has_color = false;
        ColorPallete color_palette = dark_pallete;
        Color color = dark_pallete.base;
        float darkness_shift = 0.0f;

        /** Set the default texture to new color texture. If currently default state when rendered texture updated. */
        void set_color_texture(ColorTexture _colorTexture);
        /** Set the default texture to new color texture. If currently hover state when rendered texture updated. */
        void set_hover_color_texture(ColorTexture _colorTexture);
        /** Set the default texture to new color texture. If currently active state when rendered texture updated. */
        void set_active_color_texture(ColorTexture _colorTexture);
        

        /** Set the default texture to new color. If currently default state when rendered texture updated. */
        void set_color(Color _color);
        /** Set the default texture to new color. If currently hover state when rendered texture updated. */
        // void set_color_hover(ColorTexture _colorHover);
        /** Set the default texture to new color. If currently active state when rendered texture updated. */
        // void set_color_active(ColorTexture _colorActive);



        // TREE QUERY
        bool isLeaf();
        void appendChild(Primitive* childPrimitive);
        std::vector<Primitive*> flattenTree();
        // TODO: move the tmp flat vector into primitive
        void appendtoFlatTreeNested(Primitive* _primitive);


        
        // STATE & BEHAVIOR

        PrimitiveState state = PrimitiveState::Default;
        /** Sets the state AND the corresponding texture  */
        void set_state(PrimitiveState _newState);

        virtual UiResult click();
        virtual UiResult hover_enter();
        virtual UiResult hover_exit();
        virtual UiResult grabbed(double dx, double dy);
        virtual UiResult scroll(double y_change);



        // COMPONENT INTERFACE

        /** Virtual method implemented by components. 
         *  Will try to find targeted primitive in its primitive tree at the passed x/y screen coordinates.
         *  Generally only matches primitives/components within the bounds of the component itself.
         */
        virtual UiResult try_find_target_component(double x, double y);
        /** Component specific rendering function. 
         *  Usually renders itself, all primitive children, and then render_component() of all its child components. 
         */
        virtual void render_component() {};
        /** Component specific update function. 
         *  Generally updates all constituent primitives using its bound object, and resursivly call update_component().
         */
        virtual void update_component() {};

};


typedef class PrimitiveString : public Primitive {
    public:
        std::string str = "";

        /** The font size used during generation of currently rendering string texture */
        FontSize str_fontSize = FontSize::f24;
        void str_setFontSize(FontSize _fontSize);
        // will load a texture contining the passed string into the primitives default glTexture
        void str_setString(std::string _str);

        /** Regenerate string texture if passed string is different from currently rendered string. */
        void update_str(std::string _str);
        /** Regenerate string texture if passed integers string representation is different from currently rendered string. */
        void update_str_int(int _int);
        /** Regenerate string texture if passed floating point string-representation is different from currently rendered string. */
        void update_str_double(double _double);
        PrimitiveString(std::string _str);
} PrimitiveString;




}



#endif