
#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <string>
#include <vector>


#include "uiElement.hpp"
#include "Types.hpp"


typedef struct UiFile {
    bool loaded = 0;
    std::string filePath;
    std::string fileContents;
} UiFile;


void ui_update();

void ui_updateFpsElement();
void ui_updateTrackedWorldObjectElement();

// void ui_setCharacterTextureData(std::vector<unsigned char> data, unsigned int imgWidth, unsigned int imgHeight);
void ui_createUiChar(int fontHeight, int x, int y, char chValue, Vec3 color, UiElement& _uiStringElem);
void ui_updateStringUi(int fontHeight, int x, int y, std::string str, UiElement& _uiStringElem);

void ui_createWidgets();

void ui_createUiRectange(int height, int width, int x, int y, Vec3 color, std::string elemName, std::string texturePath);


void ui_init();

void ui_reloadUi();

void ui_setWindowSize(int width, int height);

void ui_newShaderPlease(const char* vertexPath, const char* fragmentPath);

void ui_loadUiFile(const char* uiFilePath);
void ui_parseUiFileString();


void ui_detectElementClick(double x, double y);






#endif