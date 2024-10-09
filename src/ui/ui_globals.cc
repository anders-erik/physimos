

#include "ui_globals.hh"


#include "PSO_util.hpp"


namespace UI {


    float viewportTransform16[16] = {
                                    2.0f / (float)SCREEN_INIT_WIDTH, 0, 0, -1.0f,
                                    0, 2.0f / (float)SCREEN_INIT_HEIGHT, 0, -1.0f,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1,
    };


    unsigned int viewport_width = 0;
    unsigned int viewport_height = 0;

    void setViewportDimensions(unsigned int _width, unsigned int _height){
        viewport_width = _width;
        viewport_height = _height;

        viewportTransform16[0] = 2.0f / (float)viewport_width;
        viewportTransform16[5] = 2.0f / (float)viewport_height;
    }



    float squareVertices[30] = {
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
               0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // top-left
               1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
               0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
               1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    };

}