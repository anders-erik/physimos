
#include "ui.hpp"

#include <iomanip>

#include "shader.hpp"
#include "Input.hpp"
#include "bmp_loader.hpp"
#include "PSO_util.hpp"
#include "Timing.hpp"

#include "WorldScene.hpp"
#include "WorldObject.hpp"
extern WorldObject* worldTriangle2_simobj_pointer;

#include "Simulation.hpp"
extern SimState simState;

extern struct InputState InputState;


unsigned int uiVAO = 0;
unsigned int uiVBO = 0;
unsigned int uiIsCharVAO = 0;
unsigned int uiIsCharVBO = 0;

unsigned int uiTextureVAO = 0;
unsigned int uiTextureVBO = 0;


int windowWidth = 0;
int windowHeight = 0;

Shader shader;


// CHAR TEXTURING
unsigned int charTexture;

std::vector<float> isCharBuffer;

std::vector<unsigned char> charImageBuffer;
unsigned int charImgWidth = 0;
unsigned int charImgHeight = 0;

// String elements
std::vector<UiElement> fpsStringElements;


std::vector<UiElement> uiElements;
std::vector<float> uiVertexFloatBuffer;

float uiTransform16[16] = {
                                2.0f / 800, 0, 0, -1.0f,
                                0, 2.0f / 600, 0, -1.0f,
                                0, 0, 1, 0,
                                0, 0, 0, 1, };


unsigned int uiTransformLoc;

void ui_detectElementClick(double x, double y) {
    int xx = (int)x;
    int yy = (int) (windowHeight - y);
    for(UiElement _uiELem : uiElements){
        // std::cout << "UI ELEMENT CHECK!" << std::endl;
        // std::cout << "xx= " << xx << "yy= " << yy << std::endl;
        // std::cout << "_uiELem.name = " << _uiELem.name << ",   :   _uiELem.x = " << _uiELem.x << "  _uiELem.y = " << _uiELem.y << ",     _uiELem.width = " << _uiELem.width << "  _uiELem.height = " << _uiELem.height << std::endl;
        
        // Detect element click
        if (((xx > _uiELem.x) && (xx < (_uiELem.x + _uiELem.width))) && ((yy > _uiELem.y) && (yy < (_uiELem.y + _uiELem.height)))){
            // std::cout << "CLICKED : " << _uiELem.name << std::endl;
            if (_uiELem.name == "startSimButton")
                simState = SimState::startClickDetected;
        }
            
    }
}


void ui_update() {
    ui_updateFpsElement();
    ui_updateTrackedWorldObjectElement();

    ui_renderUI();
}


void ui_init() {

    // bmp_loader.loadBMPFile("media/characters-1.bmp");
    
    ui_newShaderPlease("src/shaders/ui.vs", "src/shaders/ui.fs");
    ui_setWindowSize(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT);
    ui_reloadUi();


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

void ui_setCharacterTextureData(std::vector<unsigned char> data, unsigned int imgWidth, unsigned int imgHeight) {
    charImageBuffer = data;
    charImgWidth = imgWidth;
    charImgHeight = imgHeight;
}


void ui_newShaderPlease(const char* vertexPath, const char* fragmentPath) {
    shader.buildShaderProgram(vertexPath, fragmentPath);
}

int firstRun = 1;
void ui_updateFpsElement() {
    int fps = timing_current_fps();
    fpsStringElements.clear();

    std::string fps_str = "FPS: ";
    fps_str.append(std::to_string(fps));

    // To prevent deletion of other elements on the first update
    // if(firstRun == 1){
    //     firstRun = 0;
    // }
    // else{
    //     uiElements.pop_back();
    //     uiElements.pop_back();
    //     uiElements.pop_back();
    // }

    // uiElements.pop_back();
    // uiElements.pop_back();
    // uiElements.pop_back();
    for (UiElement& _uiElem : uiElements) {
        if (_uiElem.name == "fps")
            ui_updateStringUi(15, 0, 570, fps_str, _uiElem);
    }
    // updateStringUi(15, 0, 570, fps_str, fpsStringElement);
    // updateStringUi(15, 200, 30, "A!!!123000", firstStringElement);
    // updateStringUi(15, 200, 0, "Second string! []{}()", secondStringElement);

}

int trackedWorldObjectUpdateCount = 0;
void ui_updateTrackedWorldObjectElement(){

    fpsStringElements.clear();

    std::string worldObjectTracked_str = "WORLD OBJECT TRACKED ";
    
    std::string twose_z_str = "twose_Z = ";
    twose_z_str = (*worldTriangle2_simobj_pointer).name;
    twose_z_str = twose_z_str.append(".Z = ");
    
    // Parse float to string
    float twose_z_value = (*worldTriangle2_simobj_pointer).position.z;
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << twose_z_value;
    std::string s = stream.str();
    twose_z_str = twose_z_str.append(s);
    // std::cout << "worldTriangle2_simobj_pointer = " << worldTriangle2_simobj_pointer << std::endl;
    
    for (UiElement& _uiElem : uiElements) {
        if (_uiElem.name == "trackedWorldObject")
            ui_updateStringUi(15, 0, 500, worldObjectTracked_str, _uiElem);
        if (_uiElem.name == "twose_name"){
            
            // Slow down updates
            if (trackedWorldObjectUpdateCount % 5 == 0) {
                ui_updateStringUi(15, 0, 478, twose_z_str, _uiElem);
                // std::cout << "trackedWorldObjectUpdateCount = " << trackedWorldObjectUpdateCount << std::endl;

            }
            trackedWorldObjectUpdateCount++;
        }
    }
}

void ui_reloadUi() {

    uiElements.clear();
    fpsStringElements.clear();

    uiTransform16[0] = 2.0f / windowWidth;
    uiTransform16[5] = 2.0f / windowHeight;

    // ui_createUiRectange(100, 100, 700, 500, pso_white, "w1");
    // ui_createUiRectange(100, 100, 700, 400, pso_black, "b1");
    // ui_createUiRectange(100, 100, 700, 300, pso_red, "r1");
    // ui_createUiRectange(100, 100, 700, 200, pso_green, "g1");
    // ui_createUiRectange(100, 100, 700, 100, pso_blue, "b2");

    // createUiChar(300, 0, 120, 'A', white);

    UiElement newStringELement_1("AnotherStringElem");
    ui_updateStringUi(15, 200, 570, "ANOTHER STRING!", newStringELement_1);
    uiElements.push_back(newStringELement_1);

    UiElement firstStringElement("firstStrElem");
    ui_updateStringUi(15, 200, 30, "A!!!123000", firstStringElement);
    uiElements.push_back(firstStringElement);
    UiElement secondStringElement("secondStrElem");
    ui_updateStringUi(15, 200, 0, "Second string! []{}()", secondStringElement);
    uiElements.push_back(secondStringElement);

    UiElement fpsStringElement;
    fpsStringElement.name = "fps";
    uiElements.push_back(fpsStringElement);

    UiElement trackedWorldObjectStringElement;
    trackedWorldObjectStringElement.name = "trackedWorldObject";
    uiElements.push_back(trackedWorldObjectStringElement);
    UiElement twose_name;
    twose_name.name = "twose_name";
    uiElements.push_back(twose_name);

    ui_loadUiFile("src/main.psoui");

    for(auto _elem : uiElements){
        if (_elem.name == "textureElem_1"){
            // std::cout << "_elem.elementTexturePath: " << _elem.elementTexturePath << std::endl;
            // std::cout << "_elem.elementTextureBuffer.size(): " << _elem.elementTextureBuffer.size() << std::endl;
        }
    }

}



void ui_renderUI()
{
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

void ui_setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}




void ui_loadUiFile(const char* uiFilePath) {
    // std::cout << std::endl << "Loading psoui: " << uiFilePath << std::endl;


    std::ifstream psouiFile;
    std::stringstream psouiStream;
    std::string psouiString;

    psouiFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {

        psouiFile.open(uiFilePath);


        psouiStream << psouiFile.rdbuf();

        psouiString = psouiStream.str();

        psouiFile.close();

        std::string keyValString;
        std::string key;
        std::string value;
        std::string line;
        // float number;


        // UiElement uiElem_;

        // Grab line
        while (std::getline(psouiStream, line)) {

            std::istringstream lineStream(line);

            UiElement uiElem_;

            // split line into key-value pairs
            while (std::getline(lineStream, keyValString, ';')) {

                // for(std::string KV : item){}
                // std::cout << keyValString << std::endl;

                std::string::size_type n;
                n = keyValString.find(":");

                // error parsing key-value pair
                if (n == std::string::npos) // -1 ??
                    std::cout << "ERROR::PARSING_PSOUI_FILE. " << __FILE__ << std::endl;


                // keyValString.copy(key, n);
                key = keyValString.substr(0, n);
                value = keyValString.substr(n + 1, keyValString.size());
                // std::cout << key << std::endl;
                // std::cout << value << std::endl;



                if (key == "name") {
                    uiElem_.name = value;
                }
                else if (key == "type") {
                    uiElem_.type = value;
                }
                else if (key == "x") {
                    if (value[0] == 'L')
                        uiElem_.x = std::stoi(value.substr(1, value.size()));
                    else if (value[0] == 'R')
                        uiElem_.x = windowWidth - std::stoi(value.substr(1, value.size()));
                }
                else if (key == "y") {
                    if (value[0] == 'B')
                        uiElem_.y = std::stoi(value.substr(1, value.size()));
                    else if (value[0] == 'T')
                        uiElem_.y = windowHeight - std::stoi(value.substr(1, value.size()));
                }
                else if (key == "w") {
                    uiElem_.width = std::stoi(value);
                }
                else if (key == "h") {
                    uiElem_.height = std::stoi(value);
                }
                else if (key == "texture") {
                    uiElem_.elementTexturePath = value;
                    // std::cout << "--- " ;
                    // std::cout << "" << uiElem_.elementTexturePath << std::endl;
                    
                    
                }
                else if (key == "c") {

                    if (value == "black") {
                        uiElem_.color = pso_black;
                    }
                    else if (value == "white") {
                        uiElem_.color = pso_white;
                    }
                    else if (value == "red") {
                        uiElem_.color = pso_red;
                    }
                    else if (value == "green") {
                        uiElem_.color = pso_green;
                    }
                    else if (value == "blue") {
                        uiElem_.color = pso_blue;
                    }
                }


            }

            if (uiElem_.type == "rectangle") {
                ui_createUiRectange(uiElem_.height, uiElem_.width, uiElem_.x, uiElem_.y, uiElem_.color, uiElem_.name, uiElem_.elementTexturePath);
            }

            // uiElements.push_back(uiElem_);

            // for(std::string KV : item){}
            // std::cout << item << std::endl;

            // std::stringstream itemStream(item);
            // itemStream >> number;

            // // Add the parsed float to the vector
            // this->vertexFloatBuffer.push_back(number);
        }

        std::cout << "PSUI OK. " << uiFilePath << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << " ERROR. " << uiFilePath << " : tried loading psoui file. " << std::endl;
        std::cout << "ERROR::READING_PSOUI_FILE" << e.what() << std::endl;
    }


}

void ui_createUiRectange(int height, int width, int x, int y, Vec3 color, std::string elemName, std::string _texturePath) {
    UiElement uiElem_;
    for (int i = 0; i < uiElem_.vertexCount * 8; i += 8) {
        uiElem_.vertexFloatBuffer[i] = uiElem_.squareVertices[i] * width + x;
        uiElem_.vertexFloatBuffer[i + 1] = uiElem_.squareVertices[i + 1] * height + y;

        uiElem_.vertexFloatBuffer[i + 3] = color.x;
        uiElem_.vertexFloatBuffer[i + 4] = color.y;
        uiElem_.vertexFloatBuffer[i + 5] = color.z;

        uiElem_.vertexFloatBuffer[i + 6] = uiElem_.charVertices[i + 6];
        uiElem_.vertexFloatBuffer[i + 7] = uiElem_.charVertices[i + 7];
    }

    uiElem_.activated = 1;
    uiElem_.name = elemName;
    uiElem_.x = x;
    uiElem_.y = y;
    uiElem_.width = width;
    uiElem_.height = height;
    uiElem_.elementType = ElementType::RECTANGLE;

    if(_texturePath != ""){
        uiElem_.elementType = ElementType::TEXTURE;

        uiElem_.elementTexturePath = _texturePath;
        bmp_loader_loadBMPFile(uiElem_.elementTexturePath);
        uiElem_.elementTextureBuffer = bmp_getImageDataBuffer();
        uiElem_.textureWidth = bmp_getWidth();
        uiElem_.textureHeight = bmp_getHeight();

        // Move texture to gpu
        glGenTextures(1, &uiElem_.glTexture);
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

    // uiElem_.vertices
    uiElements.push_back(uiElem_);
}


void ui_updateStringUi(int fontHeight, int x, int y, std::string str, UiElement& _uiStringElem) {
    // UiElement _uiStringElem;
    _uiStringElem.activated = 1;
    _uiStringElem.isStringElement = true;
    _uiStringElem.elementType = ElementType::STRING;
    _uiStringElem.stringElements.clear();

    _uiStringElem.x = x;
    _uiStringElem.y = y;
    _uiStringElem.width = fontHeight * str.size() * 0.66;
    _uiStringElem.height = fontHeight;

    int charCount = 0;
    int fontWidth = fontHeight / 1.20;
    for (char ch : str) {
        ui_createUiChar(fontHeight, x + charCount * fontWidth, y, ch, pso_white, _uiStringElem);
        charCount++;
    }
    // uiElements.push_back(_uiStringElem);
}


void ui_createUiChar(int fontHeight, int x, int y, char chValue, Vec3 color, UiElement& _uiStringElem) {
    UiElement uiElem_;
    for (int i = 0; i < uiElem_.vertexCount * 8; i += 8) {
        uiElem_.vertexFloatBuffer[i] = uiElem_.charVertices[i] * fontHeight + x;
        uiElem_.vertexFloatBuffer[i + 1] = uiElem_.charVertices[i + 1] * fontHeight + y;

        uiElem_.vertexFloatBuffer[i + 3] = color.x;
        uiElem_.vertexFloatBuffer[i + 4] = color.y;
        uiElem_.vertexFloatBuffer[i + 5] = color.z;

        // Determine UV coordinates for each vertex
        int column = chValue % 10;
        int row = (chValue / 10) - 3;

        uiElem_.vertexFloatBuffer[i + 6] = uiElem_.charVertices[i + 6] * 0.09 + (float)column * 0.1;
        uiElem_.vertexFloatBuffer[i + 7] = uiElem_.charVertices[i + 7] * 0.1 + 0.9 - (float)row * 0.1;
    }

    uiElem_.activated = 1;
    uiElem_.isChar = 10.0;
    

    // uiElem_.vertexFloatBuffer
    // uiElements.push_back(uiElem_);

    // fpsStringElements.push_back(uiElem_);
    _uiStringElem.stringElements.push_back(uiElem_);
}



float playVertices[] = {
    // back face
    0.7f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-left
    1.0f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f, // top-right
    1.0f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-right         
    1.0f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f, // top-right
    0.7f, 0.7f, -0.9f,  0.0f,  0.0f, 0.2f, // bottom-left
    0.7f,  1.0f, -0.9f,  0.0f,  0.0f, 0.2f,  // top-left

    // Play triangle
    0.8f, 0.9f, -1.0f,  0.0f,  1.0f, 0.0f, // top-left
    0.8f,  0.8f, -1.0f,  0.0f,  1.0f, 0.0f, // botton-left
    0.9f, 0.85f, -1.0f,  0.0f,  1.0f, 0.0f, // middle-right
};










