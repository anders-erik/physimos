#include <string>
#include <vector>
#include <iostream>

#include "glad/glad.h"

#include "res/bmp_loader.hpp"

#include "ui/font.hh"



namespace UI {


int charBitmapWidth = 100;
int charBitmapHeight = 150;

unsigned int fontBitmapWidth = 1000;
unsigned int fontBitmapHeight = 1500;

std::vector<unsigned char> fontBitmapBuffer;

unsigned int fontTexture = 0;
unsigned int a_texture = 0;


void loadFont(){

    bmp_loader_loadBMPFile("resources/textures/characters-2.bmp");
    fontBitmapBuffer = bmp_getImageDataBuffer();

    std::cout << fontBitmapBuffer.size() << "\n";



    //  CHARACTERS
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, fontTexture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fontBitmapWidth, fontBitmapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, fontBitmapBuffer.data());
    glGenerateMipmap(GL_TEXTURE_2D);



    // LOAD CHAR A

    std::vector<unsigned char> a_bitmapBuffer = {};

    size_t ascii_A = 'A';

    // map the ascii value to the character index, starting from BOTTOM LEFT
    size_t ascii_A_low = ascii_A % 10;
    size_t ascii_A_high = ascii_A - ascii_A_low;
    size_t bitmap_index = 120 - ascii_A_high + ascii_A_low;

    // which row row and column is the character on in the bitmap table
    size_t col_index = bitmap_index % 10;
    size_t row_index = bitmap_index / 10;
    // printf("col_index = %d\n", col_index);
    // printf("row_index = %d\n", row_index);

    // x & y location of top left corner from top left of bitmap
    size_t y_pixel_start = charBitmapHeight * row_index;
    size_t x_pixel_start = charBitmapWidth * col_index;

    // byte location at top left of char pixels
    size_t byte_start_index = fontBitmapWidth * y_pixel_start * 3 + x_pixel_start*3;
    // printf("byte_start_index = %d\n", byte_start_index);

    size_t current_byte_location;

    // step through the char width and height
    for (int row = 0; row < charBitmapHeight; row++)
    {
        for (int col = 0; col < charBitmapWidth; col++)
        {
            current_byte_location = byte_start_index;               // top left
            current_byte_location += row * fontBitmapWidth * 3;     // select row
            current_byte_location += col * 3;                       // select column

            a_bitmapBuffer.push_back(fontBitmapBuffer[current_byte_location+0]);
            a_bitmapBuffer.push_back(fontBitmapBuffer[current_byte_location+1]);
            a_bitmapBuffer.push_back(fontBitmapBuffer[current_byte_location+2]);
        }
    }
    // std::cout << "a_bitmapBuffer.size() = " << a_bitmapBuffer.size() << "\n";
    

    //  A
    glGenTextures(1, &a_texture);
    glBindTexture(GL_TEXTURE_2D, a_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, a_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, charBitmapWidth, charBitmapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, a_bitmapBuffer.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}







}