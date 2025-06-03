#ifndef UI_GLOBALS_HH
#define UI_GLOBALS_HH

#include "window_cursors.hh"
#include "conductor_common.hh"

namespace UI {

    class Primitive;

    /** Scaled viewport width using window content scale. 
     *  Example: framebuffer_width==1000, xscale==2 --> viewport_width==500  */
    extern unsigned int viewport_width;
    /** Scaled viewport height using window content scale. 
     *  Example: framebuffer_height==2000, xscale==2 --> viewport_width==1000 */
    extern unsigned int viewport_height;


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


    /** Pixel hieght of full char bitmap */
    typedef enum FontSize {
        f15 = 15,
        f18 = 18,
        f24 = 24,
        f32 = 32,
    } FontSize;

    typedef enum class Icon {
        Up,
        Down,
        Left,
        Right,

        ScrollVert,
        ScrollHori,

        Pan,

        Resize,

    } Icon;


    typedef enum class ColorTexture {
        Pink, // uninitialized texture
        Red,  // Common default value

        Gray,
        LightGray,
        DarkGray,

        Green,
        LightGreen,
        DarkGreen,
    } ColorTexture;


    /** Colors accepted by the ui color shader. Values are floats in range 0.0-1.0. */
    typedef struct Color {
        float R;
        float G;
        float B;
        float A;
    } Color;

    extern Color transparent;
    extern Color black;

    /** Collection of three colors. Base, contrast, and detail.  */
    typedef struct ColorPallete {
        /** Primary base color */
        Color base1;
        /** Complementary base color for contrast between primitives */
        Color base2;
        /** Primary contrasting color for text and icons for base1 color */
        Color contrast1;
        /** Contrasting color for text and icons for base2 color */
        Color contrast2;
        /** Additional detail color for highlighting */
        Color detail1;
        Color detail2; /** Maybe delete */
    } ColorPallete;

    extern ColorPallete active_pallete;

    enum class ColorPalletes {
        Dark_1,
    };

    void set_active_pallete(ColorPalletes color_pallete);
    

    // enum Action {
    //     None = 0,
    //     TogglePObjectContainer = 10,
    //     LoadPObject = 11,
    //     ReloadPObject = 12,
    // };

    typedef struct UiResult {
        bool success = false;
        CAction action = CAction::None;
        Primitive* primitive = nullptr;
        
        UiResult() {};
        UiResult(bool _success) : success {_success} {};
        UiResult(bool _success, CAction _action) : success{ _success }, action{_action} {};
        UiResult(bool _success, CAction _action, Primitive* _primitive) : success{ _success }, action{ _action }, primitive{_primitive} {};
    } UiResult;

    /** The state returned from Grab methods, indicating what the Conductor should do. */
    typedef struct GrabState {
        bool grabbed = false;
        PCursor cursor = PCursor::Default;
        Primitive* primitive = nullptr;

        GrabState() {};
        GrabState(bool _grabbed, PCursor _cursor, Primitive* _primitive) : grabbed{ _grabbed }, cursor{ _cursor }, primitive{_primitive} {};
    } GrabState;

    /** The state returned from Grab methods, indicating what the Conductor should do. */
    typedef struct HoverEvent {
        bool new_hover = false;
        PCursor cursor = PCursor::Default;
        Primitive* primitive = nullptr;

        HoverEvent() {};
        HoverEvent(bool _new_hover, PCursor _cursor, Primitive* _primitive) : new_hover{ _new_hover }, cursor{ _cursor }, primitive{_primitive} {};
    } HoverEvent;
    
}



#endif