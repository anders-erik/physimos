#ifndef UI_PRIMITIVE_HH
#define UI_PRIMITIVE_HH

#include <string>
#include <vector>

#include "math/vecmat.hh"

#include "ui/ui_shader_texture.hh"
#include "ui/ui_shader_color.hh"
#include "ui/ui_shader_string.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_font.hh"

namespace UI {

// forward declare
class Primitive; 
class RendererPrimitive;


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

enum class Unit {
    Pixel = 0,
    Percent = 1
};

struct Units {
    Unit x = Unit::Pixel;
    Unit y = Unit::Pixel;

    Units(Unit initial_units) : x {initial_units}, y{initial_units} {};
};

enum class VertRef {
    /** Distance will be measured from parent top to primitive top */
    Top = 0,
    /** Distance from primitive top to parent top will be equal to the distance from primitive bottom to parent bottom. */
    Center = 1,
    /** Distance will be measured from parent bottom to primitive bottom */
    Bottom = 2,
};

enum class HoriRef {
    /** Distance will be measured from parent left edge to primitive left edge */
    Left = 0,
    /** Distance from primitive left to parent left will be equal to the distance from primitive right to parent right. */
    Center = 1,
    /** Distance will be measured from parent right edge to primitive right edge */
    Right = 2,
};

/**  Vertical and horizontal reference position. */
class RefPos {

    VertRef vert = VertRef::Bottom;
    HoriRef hori = HoriRef::Left;

public:

    void set_vert(VertRef new_vert) {vert = new_vert;};
    void set_hori(HoriRef new_hori) {hori = new_hori;};
    VertRef get_vert() {return vert;};
    HoriRef get_hori() {return hori;};

    bool vert_is(VertRef vert_to_query) { return vert_to_query == vert ? true : false;};
    bool hori_is(HoriRef hori_to_query) { return hori_to_query == hori ? true : false;};
};


/** Specifies the full transform state of UI primitive: size, position, render transform, size change flags.  */
struct Transform {

    // Reload-flags

    /** Intially set to true for full update during first render. */
    bool size_changed = true;
    bool x_pos_changed = true;
    bool y_pos_changed = true;

    Units unit_size;
    Units unit_pos;
    RefPos ref_pos;

    // sizes and positions
    i2 input_size = {100, 100}; // Unit-independent input size
    i2 input_pos = {0, 0};      // Unit-independent input position
    i2 input_offset = {0, 0};   // Fixed offset distance following optional 'o' in input str
    i2 real_pos = {0, 0};   // position (px) : bottom left window to bottom left primitive.
    i2 real_size = {0, 0};  // size (px).
    
    // Limits
    i2 min_size = {0, 0}; // prevent primitive from shrinking below zero
    i2 max_size = {INT32_MAX, INT32_MAX};

    // Model-to-scene matrix for rendering
    m4f4 M_m_s;

    Transform() 
        :   unit_size {Unit::Percent},
            unit_pos  {Unit::Pixel} {};

};



class Primitive {
    public:
        /** Initializes a new primitive ui object. Sets shaders and iinital colors. */
        Primitive();
        /** Initializes a new primitive ui object with a specified parent. Sets shaders and iinital colors. */
        Primitive(Primitive* _parent);
        /** Initializes a new primitive ui object. Sets shader and initial string texture. Background color is set to transparent! */
        Primitive(std::string _str);
        /** Initializes a new primitive ui object with a specified parent. Sets shader and initial string texture. Background color is set to transparent! */
        // Primitive(Primitive* _parent, std::string _str);


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

        void dec_w();
        void inc_w();
        void dec_h();
        void inc_h();
        void dec_x();
        void inc_x();
        void dec_y();
        void inc_y();

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
        ::UI::shader::StringShader*      string_shader;
        bool render_enabled = true;

        /** Set the 4x4 transformation matrix that will be set as shader uniform */
        void updateTransformationMatrix();
        
        /** Before each render: 
            - dimension change flags are checked. If set, we update primitive and its descendants. */
        void try_update_box_recursive();

        /** Render current state of primitive */
        void render();
        /** Render primitive with specific renderer. */
        void render(RendererPrimitive& renderer);
        /** Render current state of primitive and making this same call to all its children
         *  Very useful for ui branches that does not need updating or component behavior, such as the string primitives of the fixed sized buttons in the topbar.
         */
        void render_recursive();
        // Primitives without parents will have a z-value of 1. Each child will recieve a z value of parent.z + 1.
        // A z value of 0 will not be rendered ?
        int z = 1;
        /** Increments the z values recursively. Use when a new parent is set and thus the z value of each descendant needs to reflect this.  */
        void update_z_recursive();

        /** Render flag */
        bool has_texture = false;
        /** Set the has_texture render falg and sets the rendered texture. */
        void set_texture(unsigned int glTextureName);
        /** Set the rendered texture to color texture. */
        void set_color_texture(ColorTexture _colorTexture);
        /** The currently rendered texture for UI::Primitive. */
        unsigned int renderedTexture = 22222222;
        /** Generated when setting a new primitive string in PrimitiveString. */
        unsigned int privateStringTexture = 22222222;


        /** The color used when rendering a primitive. This will always render, even when a texture os string has been set. */
        Color color = active_pallete.base1;
        /** Set the default texture to new color. If currently default state when rendered texture updated. */
        void set_color(Color _color);

        /** Value between 1.0 and -1.0 used to  darken the rendered color. Positive values darken. */
        float darkness_shift = 0.0f;
        /** Set color darkness shift. Value between 1.0 and -1.0. Positive values darken. */
        void set_darkness_shift(float shift_value);
        /** Resets color darkness shift, usually to 0.0f. */
        void reset_darkness_shift();




        // STRING

        /** String that is currently being rendered. No string rendered if size = 0.  */
        std::string str = "";

        /** The font size used during generation of currently rendering string texture */
        FontSize str_fontSize = FontSize::f18;
        /** Set the font size and reloads the string texture */
        void str_setFontSize(FontSize _fontSize);
        // will load a texture contining the passed string into the primitives default glTexture
        void str_setString(std::string _str);

        /** Regenerate string texture if passed string is different from currently rendered string. */
        void update_str(std::string _str);
        /** Regenerate string texture if passed integers string representation is different from currently rendered string. */
        void update_str_int(int _int);
        /** Regenerate string texture if passed floating point string-representation is different from currently rendered string. */
        void update_str_double(double _double, int chars);





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

        /** This flag has to be set for scroll to work. Used during UI bubbling to find scroll primitive.  */
        bool scrollable = false;
        virtual UiResult click();
        virtual HoverEvent hover_enter();
        virtual HoverEvent hover_exit();
        virtual GrabState grab();
        virtual GrabState grabbed(double dx, double dy);
        virtual UiResult scroll(double y_change);



        // COMPONENT INTERFACE


        /** May be used to enable stencil testing during render of descendants. 
         *  If set to true any pixel rendered by a descendant beyond the root component will be discarded.
         *  Primarily used to hide scrollable lists. 
         *  NOTE: Must be explicity enabled in the 'render_component' implementation of the specific component. 
         */
        bool stencil_test = false;

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


}



#endif