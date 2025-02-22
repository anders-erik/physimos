#ifndef UI_GLOBALS_HH
#define UI_GLOBALS_HH


namespace UI {

    class Primitive;

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
    

    enum Action {
        None = 0,
        TogglePObjectContainer = 10,
        LoadPObject = 11,
        ReloadPObject = 12,
    };

    typedef struct UiResult {
        bool success = false;
        Action action = Action::None;
        Primitive* primitive = nullptr;
        
        UiResult() {};
        UiResult(bool _success) : success {_success} {};
        UiResult(bool _success, Action _action) : success{ _success }, action{_action} {};
        UiResult(bool _success, Action _action, Primitive* _primitive) : success{ _success }, action{ _action }, primitive{_primitive} {};
    } UiResult;


    extern unsigned int viewport_width;
    extern unsigned int viewport_height;

    extern float viewportTransform16[16];


    // Colors
    extern unsigned char color_default[4];
    extern unsigned char color_hover[4];
    extern unsigned char color_selected[4];
    
}



#endif