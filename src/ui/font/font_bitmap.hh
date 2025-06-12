
#pragma once


#include "math/vecmat.hh"


namespace UI    {
namespace Font  {


const s2 
bitmap_size = {80, 15000};

const s2 
char_size   = {80, 150  };


/** The vertical bitmap begins at index 30 */
const int bitmap_ascii_offset = 30;

/** The number of chars stacked in bitmap texture. */
const float bitmap_char_count = 100;


/** Converts ascii value to the correct y-offset in vertical bitmap texture. 
    Texture coordinates assumed to be [0,1]x[0,1] */
float char_to_texture_y_offset(char ch);



} } // Font :: UI

