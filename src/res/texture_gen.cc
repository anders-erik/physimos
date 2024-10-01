
#include "texture_gen.hh"

#include <glad/glad.h>

#include <cstdlib>


namespace res {


unsigned int qrTexture = 0;


unsigned int generateRandomTexture() {


    // RANDOM TEXTURE
    glGenTextures(1, &qrTexture);
    glBindTexture(GL_TEXTURE_2D, qrTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // bmp_loader_loadBMPFile("media/mountain.bmp");
    // bmp_loader.prettyPrint();

    // RANDOM DATA GENERATION
    // Old Black and white generated texture
    // Generate a black and white test 'image'
    int blackWhiteWidth = 100;
    int blackWhiteHeight = 100;
    // Generate black and white texture : 3 * blackWhiteWidth * blackWhiteHeight
    unsigned char blackWhiteImageBuffer[30000]; // = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, };

    for (long unsigned int rgb_i = 0; rgb_i < sizeof(blackWhiteImageBuffer); rgb_i += 3) {
        // std::cout << rgb_i << " ";
        int r = rand();

        if (r < 1073741823) {
            blackWhiteImageBuffer[rgb_i] = 0;
            blackWhiteImageBuffer[rgb_i + 1] = 0;
            blackWhiteImageBuffer[rgb_i + 2] = 0;
        }
        else {
            blackWhiteImageBuffer[rgb_i] = 255;
            blackWhiteImageBuffer[rgb_i + 1] = 255;
            blackWhiteImageBuffer[rgb_i + 2] = 255;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, blackWhiteWidth, blackWhiteHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &blackWhiteImageBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    return qrTexture;

}

}