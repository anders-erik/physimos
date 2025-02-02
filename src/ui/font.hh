
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


// // Pixel height
// enum FontSize {
//     f8 = 0,
//     f12 = 1,
//     // Default font size. 16 px height.
//     f16 = 2,
//     f24 = 3,
// };


// class Fonts {

//     Fonts() {};
//     Fonts(FontList _font);



// };






void loadFont();

std::vector<unsigned char> getCharBitmap(unsigned char ascii_char);

void loadCharIntoGlTexture(unsigned int glTexture, unsigned char character);
void extractCharFromBitmap(unsigned char ascii_char);


void loadStringIntoGlTexture(unsigned int glTexture, std::string stringToRender);
std::vector<unsigned char> getStringBitmap(std::string stringToRender);
std::vector<unsigned char> getCharBitmapRow(unsigned char character, unsigned int rowIndex);

}


#endif