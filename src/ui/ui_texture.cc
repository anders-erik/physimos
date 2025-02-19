
#include <glad/glad.h>

#include "ui/ui_texture.hh"


namespace UI {
    namespace texture {

        // unsigned int pinkTexture;
        unsigned char pinkColor[4] = { 255, 0, 100, 255 };

        unsigned int redTexture;
        unsigned char redColor[4] = { 255, 0, 0, 255 };


        // GRAY
        unsigned int grayTexture;
        unsigned char grayColor[4] = { 0, 150, 0, 255 };

        unsigned int lightGrayTexture = 0;
        unsigned char lightGrayColor[4] = { 0, 200, 0, 255 };

        unsigned int darkGrayTexture;
        unsigned char darkGrayColor[4] = { 0, 100, 0, 255 };


        // GREEN
        unsigned int greenTexture;
        unsigned char greenColor[4] = { 0, 150, 0, 255 };

        unsigned int lightGreenTexture = 0;
        unsigned char lightGreenColor[4] = { 0, 200, 0, 255 };

        unsigned int darkGreenTexture;
        unsigned char darkGreenColor[4] = { 0, 100, 0, 255 };

        // Returns a new 
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

        unsigned int get_static_color_texture(UI::Colors color) {
            switch (color) {

            // case StaticTexture::Green:   return greenTexture;        break;
            // case StaticTexture::LightGreen:   return lightGreenTexture;   break;
            // case StaticTexture::DarkGreen:   return darkGreenTexture;    break;
            
            case Colors::Red:   return redTexture;        break;
            
            case Colors::Gray:   return grayTexture;        break;
            case Colors::LightGray:   return lightGrayTexture;   break;
            case Colors::DarkGray:   return darkGrayTexture;    break;

            case Colors::Green:   return greenTexture;        break;
            case Colors::LightGreen:   return lightGreenTexture;   break;
            case Colors::DarkGreen:   return darkGreenTexture;    break;

            default:
                return 0;
                break;
            }
        }

        void init_static_textures() {

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