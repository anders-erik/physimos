#ifndef UI_GLOBALS_HH
#define UI_GLOBALS_HH


namespace UI {


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