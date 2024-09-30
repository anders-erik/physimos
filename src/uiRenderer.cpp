
#include "uiRenderer.hpp"

#include <map>
#include <string>

#include "shader.hpp"

#include "bmp_loader.hpp"

Shader shader;


unsigned int uiVAO = 0;
unsigned int uiVBO = 0;
// unsigned int uiIsCharVAO = 0;
unsigned int uiIsCharVBO = 0;


// Texture caching
std::map<std::string, unsigned int> uiTextures;


unsigned int charTexture;

std::vector<unsigned char> charImageBuffer;
unsigned int charImgWidth = 0;
unsigned int charImgHeight = 0;


extern std::vector<float> uiVertexFloatBuffer;
extern std::vector<float> isCharBuffer;

extern float uiTransform16[16];


void ui_renderer_init(std::vector<UiElement>& uiElements) {
    ui_renderer_newShaderPlease("resources/shaders/ui_vert.glsl", "resources/shaders/ui_frag.glsl");


    glGenVertexArrays(1, &uiVAO);
    glGenBuffers(1, &uiVBO);

    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uiElements.size() * 36, uiVertexFloatBuffer.data(), GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(float), vertexVector.data(), GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(uiVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);



    // CHAR BUFER
    // glGenVertexArrays(1, &uiIsCharVAO);
    glGenBuffers(1, &uiIsCharVBO);
    glBindBuffer(GL_ARRAY_BUFFER, uiIsCharVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * uiElements.size(), {}, GL_STATIC_DRAW);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);


    // TEXTURE BUFFER OBJECTS
    // glGenVertexArrays(1, &uiTextureVAO);
    // glGenBuffers(1, &uiTextureVBO);
    // glBindBuffer(GL_ARRAY_BUFFER, uiTextureVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * uiElements.size(), {}, GL_STATIC_DRAW);

    // glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // glEnableVertexAttribArray(3);


    // TEXTURE
    bmp_loader_loadBMPFile("media/characters-1.bmp");
    ui_setCharacterTextureData(bmp_getImageDataBuffer(), bmp_getWidth(), bmp_getHeight());

    glGenTextures(1, &charTexture);
    glBindTexture(GL_TEXTURE_2D, charTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, charImgWidth, charImgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, charImageBuffer.data());
    // std::cout << "charImageBuffer.size(): " << charImageBuffer.size() << std::endl;
    glGenerateMipmap(GL_TEXTURE_2D);

    // glUniform1i(glGetUniformLocation(shader.ID, "charTexture"), 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}




void ui_renderer_render(std::vector<UiElement>& uiElements) {
    glUseProgram(shader.ID);
    glBindVertexArray(uiVAO);
    shader.setMat4("uiTransform", uiTransform16);




    /*
        RENDER TEXTURED UI ELEMENTS
    */
    int elemCount = 0;
    for (UiElement uiElem_ : uiElements) {

        if (uiElem_.elementType == ElementType::TEXTURE) {
            glBindTexture(GL_TEXTURE_2D, uiElem_.glTexture);
            // std::cout << "uiElem_.glTexture = " << uiElem_.glTexture << std::endl;
            

            isCharBuffer.clear();
            uiVertexFloatBuffer.erase(uiVertexFloatBuffer.begin(), uiVertexFloatBuffer.end());

            for (size_t i = 0; i < 6; i++)
                isCharBuffer.push_back(2.0);

            for (float fl_ : uiElem_.vertexFloatBuffer) {
                uiVertexFloatBuffer.push_back(fl_);
            }

            // RENDER NEW TEXTURE ELEMENT
            glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 48, uiVertexFloatBuffer.data(), GL_STATIC_DRAW);


            glBindBuffer(GL_ARRAY_BUFFER, uiIsCharVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * isCharBuffer.size(), isCharBuffer.data(), GL_STATIC_DRAW);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }




    /*
            RENDER PLAIN COLOR AND STRING UI ELEMENTS
    */

    // RESET BUFFERS FOR THIS UI RENDER CALL
    uiVertexFloatBuffer.erase(uiVertexFloatBuffer.begin(), uiVertexFloatBuffer.end());
    isCharBuffer.clear();
    elemCount = 0;

    for (UiElement uiElem_ : uiElements) {

        // Ignore and element not currently active
        if (!uiElem_.activated)
            continue;


        switch (uiElem_.elementType)
        {

        case ElementType::STRING:

            // We do not render the string element itself, but its char-elements
            for (UiElement stringElem_ : uiElem_.stringElements) {
                elemCount++;

                // Each char vertex
                for (size_t i = 0; i < 6; i++)
                    isCharBuffer.push_back(1.0);

                for (float fl_ : stringElem_.vertexFloatBuffer)
                    uiVertexFloatBuffer.push_back(fl_);

            }
            break;


        case ElementType::TEXTURE:
            /* TEXTURED NON_CHAR ELEMENTS ARE RENEDERED BELOW */
            break;


        case ElementType::RECTANGLE:

            elemCount++;

            // Each rectangle vertex
            for (size_t i = 0; i < 6; i++)
                isCharBuffer.push_back(0.0);

            for (float fl_ : uiElem_.vertexFloatBuffer) {
                uiVertexFloatBuffer.push_back(fl_);
            }

            break;


        default:
            std::cout << "UNKNOWN ELEMENT TYPE" << std::endl;
            std::cout << "uiElem_.name = " << uiElem_.name << std::endl;
            
            break;
        }


    }



    // glUniform1i(glGetUniformLocation(shader.ID, "charTexture"), 0);
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, charTexture);

    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * elemCount * 48, uiVertexFloatBuffer.data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, uiIsCharVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * isCharBuffer.size(), isCharBuffer.data(), GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, elemCount * 6);


    // unbindUnbind GL
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void ui_renderer_newShaderPlease(const char* vertexPath, const char* fragmentPath) {
    shader.buildShaderProgram(vertexPath, fragmentPath);
}


void ui_setCharacterTextureData(std::vector<unsigned char> data, unsigned int imgWidth, unsigned int imgHeight) {
    charImageBuffer = data;
    charImgWidth = imgWidth;
    charImgHeight = imgHeight;
}


void ui_renderer_setTexture(UiElement& uiElem_, std::string _texturePath){

    uiElem_.elementType = ElementType::TEXTURE;

    uiElem_.elementTexturePath = _texturePath;

    // Make sure texture is not loaded if already in VRAM
    for (const auto& _texturePair : uiTextures) {
        if (_texturePair.first == _texturePath){
            // textureLoaded = true;
            uiElem_.glTexture = _texturePair.second;
            uiElem_.hasTexture = 1;

            return;
        }
    }


    bmp_loader_loadBMPFile(uiElem_.elementTexturePath);
    uiElem_.elementTextureBuffer = bmp_getImageDataBuffer();
    uiElem_.textureWidth = bmp_getWidth();
    uiElem_.textureHeight = bmp_getHeight();

    // Move texture to gpu
    glGenTextures(1, &uiElem_.glTexture);
    uiTextures.insert({ _texturePath,  uiElem_.glTexture });

    glBindTexture(GL_TEXTURE_2D, uiElem_.glTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, uiElem_.textureWidth, uiElem_.textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, uiElem_.elementTextureBuffer.data());
    // std::cout << "charImageBuffer.size(): " << charImageBuffer.size() << std::endl;
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);



    uiElem_.hasTexture = 1;
}