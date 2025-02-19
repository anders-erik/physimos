#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "glad/glad.h"

#include "res/bmp_loader.hpp"

#include "ui/font.hh"



namespace UI {


size_t charBitmapWidth = 100;
size_t charBitmapHeight = 150;

unsigned int fontBitmapWidth = 1000;
unsigned int fontBitmapHeight = 1500;

// The raw bytes of  the font bitmap file.
// first byte is bottom left corner.
std::vector<unsigned char> fontBitmapBuffer;

std::map<unsigned char, std::vector<unsigned char>> charToBitmap;


// Read whole ascii bitmap from file and extract all characters as individual bitmaps
void loadFont(){

    bmp_loader_loadBMPFile("resources/textures/characters-2.bmp");

    fontBitmapBuffer = bmp_getImageDataBuffer();


    // LOAD CHARS BITMAPS
    // loop through all available ascii chars in table
    for (size_t ascii = ' '; ascii <= '~'; ascii++)
    {
        extractCharFromBitmap(ascii);
    }

}



// extract the bitmap of for the specific character from the full ascii bitmap table
// Stores the character bitmap in 'charToBitmap' map for quick retrieval.
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
            current_byte_location = byte_start_index;               // top left of selected character
            current_byte_location += row * fontBitmapWidth * 3;     // select row
            current_byte_location += col * 3;                       // select column

            charBitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 0]);
            charBitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 1]);
            charBitmapBuffer.push_back(fontBitmapBuffer[current_byte_location + 2]);
        }
    }

    charToBitmap.insert({ ascii_char,  charBitmapBuffer });
}




// return the bitmap of the previously already loaded/extracted character bitmaps
// TODO: handle no match case
std::vector<unsigned char> getCharBitmap(unsigned char ascii_char) {
    for (const auto& charBitmapPair : charToBitmap) {
        if (charBitmapPair.first == ascii_char) {
            return charBitmapPair.second;
        }
    }
}




// will update a gl texture with specified character bitmap
void loadStringIntoGlTexture(unsigned int& glTexture, std::string stringToRender) {

    unsigned int charCount = stringToRender.length();

    std::vector<unsigned char> stringBitmap = getStringBitmap(stringToRender);

    // std::cout << stringBitmap.size() << std::endl;
    // std::cout << charCount << std::endl;

    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, charBitmapWidth * charCount, charBitmapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, stringBitmap.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}


// return the bitmap of a containing string
// TODO: handle no match case
std::vector<unsigned char> getStringBitmap(std::string stringToRender) {

    std::vector<unsigned char> stringBitmap;

    // loop through all chars in string for every row, concatenating each char's nth row length-wise into one long string-row
    for (size_t currentRow = 0; currentRow < charBitmapHeight; currentRow++)
    {

        for (unsigned char charToRender : stringToRender)
        {
            std::vector<unsigned char> charBitmapRow = getCharBitmapRow(charToRender, currentRow);
            for (unsigned char byte : charBitmapRow) {
                stringBitmap.push_back(byte);
            }
        }

    }
    

    return stringBitmap;
}

// returns the bytes of specified row
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




}