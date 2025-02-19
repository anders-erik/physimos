
#include <glad/glad.h>

#include "ui/ui_texture.hh"


namespace UI {
    namespace texture {

        

        unsigned int greenTexture;
        unsigned char greenColor[4] = { 0, 150, 0, 255 };

        unsigned int lightGreenTexture = 0;
        unsigned char lightGreenColor[4] = { 0, 200, 0, 255 };

        unsigned int darkGreenTexture;
        unsigned char darkGreenColor[4] = { 0, 100, 0, 255 };

        unsigned int get_static_color_texture(UI::Colors color) {
            switch (color) {

            // case StaticTexture::Green:   return greenTexture;        break;
            // case StaticTexture::LightGreen:   return lightGreenTexture;   break;
            // case StaticTexture::DarkGreen:   return darkGreenTexture;    break;
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