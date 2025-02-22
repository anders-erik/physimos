
#ifndef UI_FONT_HH
#define UI_FONT_HH

#include <vector>


namespace UI {



extern unsigned int fontTexture;
extern unsigned int a_texture;

// // character texture bitmaps available
// enum FontList {
//     First = 1,
//     Second = 2,
// };


// Pixel height
typedef enum FontSize {
    f15 = 15,
    f18 = 18,
    // Default font size. 24 px height.
    f24 = 24,
    f32 = 32,
} FontSize;



void loadFont();

void extractCharFromBitmap(unsigned char ascii_char);
std::vector<unsigned char> getCharBitmap(unsigned char ascii_char);

void loadStringIntoGlTexture(unsigned int& glTexture, std::string stringToRender);
std::vector<unsigned char> getStringBitmap(std::string stringToRender);
std::vector<unsigned char> getCharBitmapRow(unsigned char character, unsigned int rowIndex);

}


#endif