#pragma once

#include <random>

#include "glad/glad.h"


#include "texture.hh"

namespace opengl {

unsigned int colors_texture_id;
unsigned int grass_texture_id;


void textures_init(){

    // COLORS

    unsigned char colors_texture_data[] = {
        255, 255, 255, 255,
        0,   0,   0,   255, 
        
        255, 0,   0,   255, 
        0,   255, 0,   255, 
        0,   0,   255, 255,

        200, 200, 200, 255, 

        150, 100, 100, 255, 
        150, 100, 100, 255, 
        150, 100, 100, 255, 
        150, 100, 100, 255
    };

    glGenTextures(1, &colors_texture_id);
    glBindTexture(GL_TEXTURE_2D, colors_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 10, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors_texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



    unsigned char grass_texture_data[] = {
        10, 200, 10, 255,
        10, 150, 10, 255,
        10, 100, 10, 255,
        10, 150, 10, 255
    };

    glGenTextures(1, &grass_texture_id);
    glBindTexture(GL_TEXTURE_2D, grass_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass_texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


}

f2 texture_get_random_grass_color(int seed){


    // stdlib rnadom generator 
    srand(seed);
    int rand_int = rand();

    int value_to_switch = (rand_int) % 4;

    // My own stuff
    // float constant = 123942343;
    // // float x = 123942343 / val;
    // int color_count = 20;
    // float step_size = 1.0f / (float) color_count;
    // int mod = rand_int % color_count;
    // float x = step_size * (float) mod;
    // return { x, 0.5};


    
    switch(value_to_switch){
        case 0:
            return {0.125, 0.5};
        case 1:
            return {0.375, 0.5};
        case 2:
            return {0.625, 0.5};
        case 3:
            return {0.875, 0.5};
    
    }

    return {0.0, 0.0};

}

f2 texture_get_color_coordinate(TextColor text_color){

    f2 tex_coords;

    switch (text_color){

        case TextColor::Black:
            tex_coords = {0.05f, 0.5f};
            break;

        case TextColor::White:
            tex_coords = {0.15f, 0.5f};
            break;

        case TextColor::Red:
            tex_coords = {0.25f, 0.5f};
            break;
        
        case TextColor::Green:
            tex_coords = {0.35f, 0.5f};
            break;

        case TextColor::Blue:
            tex_coords = {0.45f, 0.5f};
            break;

        case TextColor::LightGray:
            tex_coords = {0.55f, 0.5f};
            break;
    }

    return tex_coords;
}

unsigned int texture_get_id(Texture texture) {
    switch (texture){

        case Texture::Colors:
            return colors_texture_id;
            break;
        case Texture::Grass:
            return grass_texture_id;
            break;
        default:
            return colors_texture_id;
            break;
    }
}

}