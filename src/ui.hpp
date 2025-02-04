
#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <string>
#include <vector>


#include "uiElement.hpp"
#include "Types.hpp"

#include "ui/ui_primitive.hh"
#include "ui/ui_globals.hh"


typedef struct UiFile {
    bool loaded = 0;
    std::string filePath;
    std::string fileContents;
} UiFile;


namespace UI {

    void pointerPositionCallback(double x, double y);


    UI::Action updatePObjectPosition();

    Primitive* getTargetingPrimitive();

    void setCurrentlyHoveredPrimitive(Primitive* primitiveToHover);

}

void ui_update();

void ui_createWidgets();
void ui_updateWidgets();

void ui_updateTimerWidget();
void ui_updateWorldObjectWidget();
void ui_updateTrackedWorldObjectElement();

// void ui_setCharacterTextureData(std::vector<unsigned char> data, unsigned int imgWidth, unsigned int imgHeight);
void ui_createUiChar(int fontHeight, int x, int y, char chValue, Vec3 color, UiElement& _uiStringElem);
void ui_updateStringUi(int fontHeight, int x, int y, std::string str, UiElement& _uiStringElem);



void ui_createUiRectange(int height, int width, int x, int y, Vec3 color, std::string elemName, std::string texturePath);


void ui_init();

void ui_reloadUi();

void ui_setWindowSize(int width, int height);

void ui_newShaderPlease(const char* vertexPath, const char* fragmentPath);

void ui_loadUiFile(const char* uiFilePath);
void ui_parseUiFileString();


void ui_detectElementClick(double x, double y);






#endif