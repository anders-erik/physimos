#pragma once

#include "math/vecmat.hh"


typedef unsigned int uint;

using TextureColor = int;

namespace opengl {


struct TextureUnits
{
    enum Name
    {
        O = 0, // Zero
        Font = 1,
        Icons = 2,
        Color = 3,
    };

    /*  Number of available color steps in each of R, G & B.
        Total # colors: Resolution ^ 3. (e.g. 16^3 = 4096)*/
    static const uint ColorResolution = 8;

    void init();

    static f2 get_color_texco(TextureColor uint_color);

private:

    void init_font();
    void init_color();

};


}