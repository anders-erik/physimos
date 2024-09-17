#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

#include <vector>

#include <glad/glad.h>

#include "uiElement.hpp"


void ui_renderer_init(std::vector<UiElement>& uiElements);
void ui_renderer_render(std::vector<UiElement>& uiElements);

void ui_renderer_newShaderPlease(const char* vertexPath, const char* fragmentPath);
void ui_setCharacterTextureData(std::vector<unsigned char> data, unsigned int imgWidth, unsigned int imgHeight);

void ui_renderer_setTexture(UiElement& uiElem_, std::string _texturePath);

#endif