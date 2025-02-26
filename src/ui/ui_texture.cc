#include <vector>
#include <iostream>

#include <glad/glad.h>

#include "ui/ui_texture.hh"
#include "image/bmp.hh"
#include "lib/process.hh"



namespace UI {
    namespace texture {

        // unsigned int pinkTexture;
        unsigned char pinkColor[4] = { 255, 0, 100, 255 };

        unsigned int redTexture;
        unsigned char redColor[4] = { 255, 0, 0, 255 };


        // GRAY
        unsigned int darkGrayTexture;
        unsigned char darkGrayColor[4] = { 100, 100, 100, 255 };

        unsigned int grayTexture;
        unsigned char grayColor[4] = { 150, 150, 150, 255 };

        unsigned int lightGrayTexture;
        unsigned char lightGrayColor[4] = { 200, 200, 200, 255 };


        // GREEN
        unsigned int darkGreenTexture;
        unsigned char darkGreenColor[4] = { 0, 100, 0, 255 };

        unsigned int greenTexture;
        unsigned char greenColor[4] = { 0, 150, 0, 255 };

        unsigned int lightGreenTexture = 0;
        unsigned char lightGreenColor[4] = { 0, 200, 0, 255 };


        unsigned int new_from_bitmap(pimage::Bitmap& bitmap){
            
            unsigned int newTexture = 0;

            glGenTextures(1, &newTexture);
            glBindTexture(GL_TEXTURE_2D, newTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, newTexture);


            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels.data());
            glGenerateMipmap(GL_TEXTURE_2D);

            return newTexture;
        }

        void update_with_bitmap(unsigned int textureName, pimage::Bitmap& bitmap){
            // glGenTextures(1, &textureName);
            glBindTexture(GL_TEXTURE_2D, textureName);

            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);


            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, textureName);


            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels.data());
            glGenerateMipmap(GL_TEXTURE_2D);
        }


        void new_texture(unsigned int& texture_to_init) {

            // unsigned int new_pink_texture;

            int imageBufferWidth = 1;
            int imageBufferHeight = 1;
            unsigned char colorBuffer[4] = { 255, 0, 100, 255 };

            glGenTextures(1, &texture_to_init);
            glBindTexture(GL_TEXTURE_2D, texture_to_init);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, texture_to_init);


            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);

        }

        unsigned int get_static_color_texture(UI::ColorTexture color) {
            switch (color) {

            case ColorTexture::Red:   return redTexture;        break;
            
            case ColorTexture::Gray:   return grayTexture;        break;
            case ColorTexture::LightGray:   return lightGrayTexture;   break;
            case ColorTexture::DarkGray:   return darkGrayTexture;    break;

            case ColorTexture::Green:   return greenTexture;        break;
            case ColorTexture::LightGreen:   return lightGreenTexture;   break;
            case ColorTexture::DarkGreen:   return darkGreenTexture;    break;

            default:
                return 0;
                break;
            }
        }



        unsigned int upTexture;
        unsigned int downTexture;
        unsigned int leftTexture;
        unsigned int rightTexture;

        unsigned int scrollVertTexture;
        unsigned int scrollHoriTexture;
        unsigned int panTexture;

        unsigned int resizeTexture;

        unsigned int get_icon(UI::Icon icon){

            switch (icon) {

                
                case Icon::Up:              return upTexture;        break;
                case Icon::Down:            return downTexture;   break;
                case Icon::Left:            return leftTexture;    break;
                case Icon::Right:           return rightTexture;    break;

                case Icon::ScrollVert:      return scrollVertTexture;        break;
                case Icon::ScrollHori:      return scrollHoriTexture;   break;
                
                case Icon::Pan:             return panTexture;        break;

                case Icon::Resize:             return resizeTexture;        break;

                default: return 0; break;
            }

            return 0;
        }

        void init_static_icon_textures(){
            
            // Icons are 20x20
            int icon_width = 20;
            int icon_height = 20;

            std::string phys_dir = physimos_root_dir_or_die();

            // unsigned char colorBuffer[16] = {   0, 0, 0, 255,
            //                                     0, 0, 255, 255,
            //                                     255, 255, 255, 255,
            //                                     255, 255, 255, 255
            // };

            // pimage::Bitmap* up_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/pan.bmp");
            // pimage::Bitmap* up_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/4x4.bmp");
            // std::vector<pimage::Pixel> pixels = up_bitmap->pixels;
            // pimage::Pixel* pixel_data = pixels.data();
            // unsigned char* pixels_data_raw = (unsigned char*) pixel_data;
            // Print pixels
            // for(int i = 0; i < image_width*image_height*4; i++){
            //     unsigned char tmp_char = *(pixels_data_raw+i);
            //     std::cout << i << " " << std::to_string(tmp_char) << std::endl;
            // }


            pimage::io::BMP bmp_loader = pimage::io::BMP();
            // Pointer to data used during opengl call
            pimage::Pixel* pixels_data_raw;

            //  UP
            pimage::Bitmap up_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/up.bmp");
            up_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
            // Don't need to cast to char, nor invert image as opengl expects first pixel at lower left corner
            pixels_data_raw =  up_bitmap.pixels.data();
            glGenTextures(1, &upTexture);
            glBindTexture(GL_TEXTURE_2D, upTexture);
            // RENDER TEXTURE BY GRABBING THE NEAREST COLOR VALUE. NO BLENDING.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
            glGenerateMipmap(GL_TEXTURE_2D);


            //  DOWN
            pimage::Bitmap down_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/down.bmp");
            down_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
            pixels_data_raw =  down_bitmap.pixels.data();
            glGenTextures(1, &downTexture);
            glBindTexture(GL_TEXTURE_2D, downTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
            glGenerateMipmap(GL_TEXTURE_2D);

            //  LEFT
            pimage::Bitmap left_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/left.bmp");
            left_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
            pixels_data_raw =  left_bitmap.pixels.data();
            glGenTextures(1, &leftTexture);
            glBindTexture(GL_TEXTURE_2D, leftTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
            glGenerateMipmap(GL_TEXTURE_2D);


            //  RIGHT
            pimage::Bitmap right_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/right.bmp");
            right_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
            pixels_data_raw =  right_bitmap.pixels.data();
            glGenTextures(1, &rightTexture);
            glBindTexture(GL_TEXTURE_2D, rightTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
            glGenerateMipmap(GL_TEXTURE_2D);




            //  SCROLL VERT
            pimage::Bitmap scroll_vert_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/scroll-vert.bmp");
            scroll_vert_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
            pixels_data_raw =  scroll_vert_bitmap.pixels.data();
            glGenTextures(1, &scrollVertTexture);
            glBindTexture(GL_TEXTURE_2D, scrollVertTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
            glGenerateMipmap(GL_TEXTURE_2D);


            //  SCROLL HORI
            pimage::Bitmap scroll_hori_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/scroll-hori.bmp");
            scroll_hori_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
            pixels_data_raw =  scroll_hori_bitmap.pixels.data();
            glGenTextures(1, &scrollHoriTexture);
            glBindTexture(GL_TEXTURE_2D, scrollHoriTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
            glGenerateMipmap(GL_TEXTURE_2D);



            //  PAN
            pimage::Bitmap pan_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/pan.bmp");
            pan_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
            pixels_data_raw =  pan_bitmap.pixels.data();
            glGenTextures(1, &panTexture);
            glBindTexture(GL_TEXTURE_2D, panTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
            glGenerateMipmap(GL_TEXTURE_2D);

            //  RESIZE
            pimage::Bitmap resize_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/resize.bmp");
            resize_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
            pixels_data_raw =  resize_bitmap.pixels.data();
            glGenTextures(1, &resizeTexture);
            glBindTexture(GL_TEXTURE_2D, resizeTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
            glGenerateMipmap(GL_TEXTURE_2D);

        }

        void init_static_color_textures() {

            int imageBufferWidth = 1;
            int imageBufferHeight = 1;
            unsigned char colorBuffer[4]; // = { 255, 255, 255, 255 };


            //  RED
            glGenTextures(1, &redTexture);
            glBindTexture(GL_TEXTURE_2D, redTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, redTexture);

            colorBuffer[0] = redColor[0];
            colorBuffer[1] = redColor[1];
            colorBuffer[2] = redColor[2];
            colorBuffer[3] = redColor[3];


            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);



            //  GRAY
            glGenTextures(1, &grayTexture);
            glBindTexture(GL_TEXTURE_2D, grayTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, grayTexture);

            colorBuffer[0] = grayColor[0];
            colorBuffer[1] = grayColor[1];
            colorBuffer[2] = grayColor[2];
            colorBuffer[3] = grayColor[3];


            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);


            //  LIGHT GRAY
            glGenTextures(1, &lightGrayTexture);
            glBindTexture(GL_TEXTURE_2D, lightGrayTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, lightGrayTexture);

            colorBuffer[0] = lightGrayColor[0];
            colorBuffer[1] = lightGrayColor[1];
            colorBuffer[2] = lightGrayColor[2];
            colorBuffer[3] = lightGrayColor[3];


            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);



            //  DARK GRAY
            glGenTextures(1, &darkGrayTexture);
            glBindTexture(GL_TEXTURE_2D, darkGrayTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, darkGrayTexture);

            colorBuffer[0] = darkGrayColor[0];
            colorBuffer[1] = darkGrayColor[1];
            colorBuffer[2] = darkGrayColor[2];
            colorBuffer[3] = darkGrayColor[3];


            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);





            //  GREEN
            glGenTextures(1, &greenTexture);
            glBindTexture(GL_TEXTURE_2D, greenTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, greenTexture);

            colorBuffer[0] = greenColor[0];
            colorBuffer[1] = greenColor[1];
            colorBuffer[2] = greenColor[2];
            colorBuffer[3] = greenColor[3];


            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);


            //  LIGHT GREEN
            glGenTextures(1, &lightGreenTexture);
            glBindTexture(GL_TEXTURE_2D, lightGreenTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, lightGreenTexture);

            colorBuffer[0] = lightGreenColor[0];
            colorBuffer[1] = lightGreenColor[1];
            colorBuffer[2] = lightGreenColor[2];
            colorBuffer[3] = lightGreenColor[3];


            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);



            //  DARK GREEN
            glGenTextures(1, &darkGreenTexture);
            glBindTexture(GL_TEXTURE_2D, darkGreenTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, darkGreenTexture);

            colorBuffer[0] = darkGreenColor[0];
            colorBuffer[1] = darkGreenColor[1];
            colorBuffer[2] = darkGreenColor[2];
            colorBuffer[3] = darkGreenColor[3];


            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);


        }


    }
}