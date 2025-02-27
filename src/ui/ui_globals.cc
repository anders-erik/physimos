

#include "ui_globals.hh"


#include "PSO_util.hpp"


namespace UI {
    // pink
    // unsigned char pink[4] = {255, 0,  100, 255};

    unsigned int viewport_width = 0;
    unsigned int viewport_height = 0;

    Color transparent = {0.0, 0.0, 0.0, 0.0};
    Color black = {0.0, 0.0, 0.0, 1.0};


    Color dark_light = {0.3, 0.3, 0.3, 1.0};
    Color dark_dark = {0.2, 0.2, 0.2, 1.0};
    Color light_light = {0.8, 0.8, 0.8, 1.0};
    Color light_dark = {0.8, 0.8, 0.8, 1.0};
    Color green_dark = {0.0, 0.4, 0.0, 1.0};
    Color green_light = {0.0, 0.8, 0.0, 1.0};

    ColorPallete dark_pallete = {
        dark_light,
        dark_dark,
        light_light,
        light_dark,
        green_dark,
        green_light,
    };

    // 
    ColorPallete active_pallete = dark_pallete;

    void set_active_pallete(ColorPalletes color_pallete){

        switch (color_pallete){

        case ColorPalletes::Dark_1:
            active_pallete = dark_pallete;
            break;
        
        default:
            break;
        }
        
    }


}