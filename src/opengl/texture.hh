#pragma once

#include "math/vecmat.hh"

namespace opengl {

enum class TextColor : unsigned int {
    Black,
    White,

    Red,
    Green,
    Blue,


    LightGray,
};


enum class Texture : unsigned int {
    Colors,
    Grass, 
};



void textures_init();
unsigned int texture_get_id(Texture texture);
f2 texture_get_color_coordinate(TextColor text_color);
f2 texture_get_random_grass_color(int seed);

}