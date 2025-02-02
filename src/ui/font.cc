#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "glad/glad.h"

#include "res/bmp_loader.hpp"

#include "ui/font.hh"



namespace UI {

// std::map<unsigned char, unsigned char *> charToBitmap;

size_t charBitmapWidth = 100;
size_t charBitmapHeight = 150;

unsigned int fontBitmapWidth = 1000;
unsigned int fontBitmapHeight = 1500;

std::vector<unsigned char> fontBitmapBuffer;
unsigned int fontTexture = 0;

std::vector<unsigned char> a_bitmapBuffer = {};
unsigned int a_texture = 0;

std::map<unsigned char, std::vector<unsigned char>> charToBitmap;


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

    // loop through all available ascii chars in table
    for (size_t ascii = ' '; ascii <= '~'; ascii++)
    {
        extractCharFromBitmap(ascii);
        printf("%d\n", ascii);
    }
    
    // extractCharFromBitmap('A');
    
    a_bitmapBuffer = getCharBitmap('A');
    

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

// will update a gl texture with specified character bitmap
void loadCharIntoGlTexture(unsigned int glTexture, unsigned char characterToRender){

    std::vector<unsigned char> charBitmap = getCharBitmap(characterToRender);

    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, charBitmapWidth, charBitmapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, charBitmap.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}


// return the bitmap of the previously already loaded/extracted character bitmaps
// TODO: handle no match case
std::vector<unsigned char> getCharBitmap(unsigned char ascii_char) {
    for (const auto& charBitmapPair : charToBitmap) {
        if (charBitmapPair.first == ascii_char) {
            // a_bitmapBuffer = charBitmapPair.second;
            // break ;
            return charBitmapPair.second;
        }
    }
}




// will update a gl texture with specified character bitmap
void loadStringIntoGlTexture(unsigned int glTexture, std::string stringToRender) {

    unsigned int charCount = stringToRender.length();

    std::vector<unsigned char> stringBitmap = getStringBitmap(stringToRender);

    std::cout << stringBitmap.size() << std::endl;
    std::cout << charCount << std::endl;

    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, charBitmapWidth * charCount, charBitmapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, stringBitmap.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}


// return the bitmap of a containing string
// TODO: handle no match case
std::vector<unsigned char> getStringBitmap(std::string stringToRender) {

    std::vector<unsigned char> stringBitmap;

    // loop through all chars for every row, concatenating the char bitmaps length-wise
    for (size_t currentRow = 0; currentRow < charBitmapHeight; currentRow++)
    {


        for (unsigned char charToRender : stringToRender)
        {
            // std::cout << charToRender << std::endl;
            // std::vector<unsigned char> charBitmap = getCharBitmap(charToRender);
            // return charBitmap;

            std::vector<unsigned char> charBitmapRow = getCharBitmapRow(charToRender, currentRow);
            for (unsigned char byte : charBitmapRow) {
                stringBitmap.push_back(byte);
            }
        }

    }
    

    return stringBitmap;
}

std::vector<unsigned char> getCharBitmapRow(unsigned char character, unsigned int rowIndex){
    // size_t charBitmapWidth = 100;
    size_t startingByte = charBitmapWidth * 3 * rowIndex;
    
    std::vector<unsigned char> row = {};
    std::vector<unsigned char> charBitmap = getCharBitmap(character);

    for (size_t byteIndex = startingByte; byteIndex < startingByte + charBitmapWidth*3; byteIndex++)
    {
        row.push_back(charBitmap[byteIndex]);
    }
    return row;
    
}




// extract the bitmap of for the specific character frmo the full ascii bitmap table
// Stores the character bitmap in 'charToBitmap' map for quick retrieval
void extractCharFromBitmap(unsigned char ascii_char) {

    // new vector will hold the char bitmap throughout the process lifetime
    std::vector<unsigned char> charBitmapBuffer = {};

    // Ascii chars start at 30 in bitmap table, starting at BOTTOM LEFT
    size_t bitmapAsciiIndex = ascii_char - 30;

    // which row row and column is the character on in the bitmap table
    size_t col_index = bitmapAsciiIndex % 10;
    size_t row_index = bitmapAsciiIndex / 10;
    // printf("col_index = %d\n", col_index);
    // printf("row_index = %d\n", row_index);

    // x & y location of top left corner from top left of bitmap
    size_t y_pixel_start = charBitmapHeight * row_index;
    size_t x_pixel_start = charBitmapWidth * col_index;

    // byte location at top left of char pixels
    size_t byte_start_index = fontBitmapWidth * y_pixel_start * 3 + x_pixel_start * 3;
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

            // a_bitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 0]);
            // a_bitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 1]);
            // a_bitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 2]);

            charBitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 0]);
            charBitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 1]);
            charBitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 2]);
        }
    }
    // charToBitmap.insert({ ascii_char,  a_bitmapBuffer });
    charToBitmap.insert({ ascii_char,  charBitmapBuffer });

    // std::cout << "a_bitmapBuffer.size() = " << a_bitmapBuffer.size() << "\n";

}




}