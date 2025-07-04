#include "glad/glad.h"

#include "io/bmp/bmp.hh"
#include "lib/process.hh"

#include "opengl/texture_unit.hh"


static uint colors_texture_id;
static uint font_texture_id;

void opengl::TextureUnits::
init()
{
    init_font();
    init_color();
}


f2 opengl::TextureUnits::
get_color_texco(TextureColor texture_color)
{
    // Extracted uchar value from texture color
    uchar ux = (uchar) (texture_color >> 16 );
    uchar uy = (uchar) (texture_color >> 8  );
    uchar uz = (uchar) (texture_color >> 0  );

    float color_res_f = (float) ColorResolution;
    
    // Step size in texture coordinates [0, 1]
    float x_step_size = 1.0f / color_res_f;
    float z_step_size = 1.0f / color_res_f;
    float y_step_size = 1.0f / (color_res_f * color_res_f);

    // component index : INT_color_res = {0, 1, ... , color_resolution}
    int x_i   = (float)ux / 255.1f * color_res_f;
    int y_i   = (float)uy / 255.1f * color_res_f;
    int z_i   = (float)uz / 255.1f * color_res_f;


    float x_coord =     x_i * x_step_size 
                    +   x_step_size / 2.0f;

    float yz_coord =    z_i * z_step_size
                    +   y_i * y_step_size
                    +   y_step_size / 2.0f;

    return {x_coord, yz_coord};
}



void opengl::TextureUnits::
init_font()
{
    Bitmap bitmap = {Bitmap(0,0)};

    BMP::File BMP_loader;

    std::string phys_dir = physimos_root_dir_or_die();
    std::filesystem::path path(phys_dir + "/resources/ui/font/characters-2-tall.bmp");
    
    bitmap = BMP_loader.load(path);

    // Replace black pixels with transparency
    bitmap.replace_color({0,0,0,255}, {0,0,0,0});


    glGenTextures(1, &font_texture_id);
    
    glActiveTexture(GL_TEXTURE0 + TextureUnits::Font);
    glBindTexture(GL_TEXTURE_2D, font_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(   GL_TEXTURE_2D, 
                    0, 
                    GL_RGBA, 
                    bitmap.width, 
                    bitmap.height, 
                    0, 
                    GL_RGBA, 
                    GL_UNSIGNED_BYTE, 
                    bitmap.pixels.data()    );

    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
}



void opengl::TextureUnits::
init_color()
{
    uint X = ColorResolution;
    uint Y = ColorResolution;
    uint Z = ColorResolution;

    // Never use heap. If stack smashes: use custom colors or textures.
    f3 colors_texture_data[X*Y*Z];

    #define I(x, y, z) ((x) + (y)*X + (z)*X*Y)

    for(uint zi = 0; zi < Z; zi++)
    {
        for(uint yi = 0; yi < Y; yi++)
        {
            for(uint xi = 0; xi < X; xi++)
            {
                float xc = (float)xi / (float)X;
                float yc = (float)yi / (float)Y;
                float zc = (float)zi / (float)Z;
                
                // Round the last coordinate for pure max colors - e.g. white - at expense of double color change
                if(xi == X-1)
                    xc = 1.0f;
                if(yi == Y-1)
                    yc = 1.0f;
                if(zi == Z-1)
                    zc = 1.0f;

                colors_texture_data[I(xi, yi, zi)] = {xc, yc, zc};
            }
        }
    }

    glGenTextures(1, &colors_texture_id);
    glActiveTexture(GL_TEXTURE0 + TextureUnits::Color);
    glBindTexture(GL_TEXTURE_2D, colors_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, X, Y*Z, 0, GL_RGB, GL_FLOAT, colors_texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glActiveTexture(GL_TEXTURE0);

}