#ifndef UI_GLOBALS_HH
#define UI_GLOBALS_HH


namespace UI {

    class Primitive;


    typedef struct Color {
        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;
    } Color;
    

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

    void setViewportDimensions(unsigned int _width, unsigned int _height);

    extern float squareVertices[30];

    // Colors
    extern unsigned char color_default[4];
    extern unsigned char color_hover[4];
    extern unsigned char color_selected[4];
    
}



#endif