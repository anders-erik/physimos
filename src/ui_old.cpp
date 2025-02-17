
#include "ui_old.hpp"

#include "uiRenderer.hpp"
#include "ui/ui_globals.hh"
// REFACTOR
#include "ui/font.hh"
#include "ui/ui_primitive.hh"
#include "ui/ui_list.hh"
#include "ui/UiPObject.hh"
#include "ui/UiPScene.hh"
// std::vector<UI::List*> uiLists;



#include <iomanip>

#include "Input.hpp"
#include "res/bmp_loader.hpp"
#include "PSO_util.hpp"
#include "Timing.hpp"

#include "WorldScene.hpp"
#include "WorldObject.hpp"
extern PObject* worldTriangle2_simobj_pointer;

#include "Simulation.hpp"
extern SimState simState;

extern struct InputState InputState;




// unsigned int uiVAO = 0;
// unsigned int uiVBO = 0;
// // unsigned int uiIsCharVAO = 0;
// unsigned int uiIsCharVBO = 0;

// unsigned int uiTextureVAO = 0;
// unsigned int uiTextureVBO = 0;


UiFile uiFile_main;


// Gets updated by glfw in local method
int windowWidth = 0;
int windowHeight = 0;




// CHAR TEXTURING
// unsigned int charTexture;

std::vector<float> isCharBuffer;

// std::vector<unsigned char> charImageBuffer;
// unsigned int charImgWidth = 0;
// unsigned int charImgHeight = 0;


// TIMER WIDGET
bool timerWidgetIsOn = false;
// std::vector<UiElement> fpsStringElements;
std::vector<UiElement> frameCountStringElements;
std::vector<UiElement> worldTimeStringElements;
std::vector<UiElement> worldEpochStringElements;

// WORLD OBJECT WIDGET
bool worldObjectWidgetIsOn = false;
std::vector< std::vector<UiElement> > worldObjectsWidgetArray;

// TRACKED OBJECT WIDGET
bool trackedWorldObjectWidgetIsOn = false;


std::vector<UiElement> uiElements;
std::vector<float> uiVertexFloatBuffer;

float uiTransform16[16] = {
                                2.0f / 800, 0, 0, -1.0f,
                                0, 2.0f / 600, 0, -1.0f,
                                0, 0, 1, 0,
                                0, 0, 0, 1, };


unsigned int uiTransformLoc;

// Called from the primary glfw input callback!
void ui_detectElementClick(double x, double y) {
    int xx = (int)x;
    int yy = (int)(windowHeight - y);
    for (UiElement _uiELem : uiElements) {
        // std::cout << "UI ELEMENT CHECK!" << std::endl;
        // std::cout << "xx= " << xx << "yy= " << yy << std::endl;
        // std::cout << "_uiELem.name = " << _uiELem.name << ",   :   _uiELem.x = " << _uiELem.x << "  _uiELem.y = " << _uiELem.y << ",     _uiELem.width = " << _uiELem.width << "  _uiELem.height = " << _uiELem.height << std::endl;

        // Detect element click
        if (((xx > _uiELem.x) && (xx < (_uiELem.x + _uiELem.width))) && ((yy > _uiELem.y) && (yy < (_uiELem.y + _uiELem.height)))) {
            // std::cout << "CLICKED : " << _uiELem.name << std::endl;

            // Do stuff to elements with specific names

            if (_uiELem.name == "greenSquare")
                simState = SimState::startClickDetected;
            if (_uiELem.name == "startSimButton")
                timing_startWorldTime();
            if (_uiELem.name == "pauseSimButton") {
                timing_pauseWorldTime();
            }

            if (_uiELem.name == "startSimCont") {
                std::cout << "STARTING SIM CONTAINER!" << std::endl;

            }

            if (_uiELem.name == "stopSimButton") {
                timing_resetWorldEpoch();
                timing_pauseWorldTime();
                ws_resetWorldObjects();
            }
            if (_uiELem.name == "timerWidget_toggle") {
                // std::cout << "TIMER WIDGET!" << std::endl;
                timerWidgetIsOn = timerWidgetIsOn ? false : true;
                for (UiElement& _UiE : uiElements) {
                    if (_UiE.name == "fps" || _UiE.name == "frame_count" || _UiE.name == "world_time" || _UiE.name == "world_epoch") {
                        if (_UiE.activated == 0)
                            _UiE.activated = 1;
                        else
                            _UiE.activated = 0;
                    }
                }
            }
            if (_uiELem.name == "worldObjects_toggle") {
                // std::cout << "TIMER WIDGET!" << std::endl;
                std::vector<PObject*> _worldObjects = ws_getWorldObjects();
                worldObjectWidgetIsOn = worldObjectWidgetIsOn ? false : true;

                for (PObject* _wo_p : _worldObjects) {
                    PObject& _wo = *_wo_p;
                    for (UiElement& _UiE : uiElements) {
                        if (_wo.name == _UiE.name || _UiE.name == "WORLD_OBJECTS_HEADER") {
                            if (_UiE.activated == 0)
                                _UiE.activated = 1;
                            else
                                _UiE.activated = 0;
                        }
                    }
                }
            }
            if (_uiELem.name == "trackedWorldObject_toggle") {
                // std::cout << "TIMER WIDGET!" << std::endl;
                trackedWorldObjectWidgetIsOn = trackedWorldObjectWidgetIsOn ? false : true;

                for (UiElement& _UiE : uiElements) {
                    if (_UiE.name == "trackedWorldObject" || _UiE.name == "twose_name") {
                        if (_UiE.activated == 0)
                            _UiE.activated = 1;
                        else
                            _UiE.activated = 0;
                    }
                }
            }

            if (worldObjectWidgetIsOn) {
                std::vector<PObject*> _worldObjects = ws_getWorldObjects();

                for (PObject* _wo_p : _worldObjects) {
                    PObject& _wo = *_wo_p;
                    if (_wo.name == _uiELem.name) {
                        // _wo.isRendpipe = _wo.isRendpipe ? false : true; // toggle visibility
                        _wo.toggleWireframe();

                    }
                    // for (UiElement& _UiE : uiElements) {
                        // if (_wo.name == _UiE.name || _UiE.name == "WORLD_OBJECTS_HEADER") {
                        //     if (_UiE.activated == 0)
                        //         _UiE.activated = 1;
                        //     else
                        //         _UiE.activated = 0;
                        // }
                    // }
                }
            }


        }

    }
}



void ui_init() {
    ui_setWindowSize(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT);

    // load into memory
    ui_loadUiFile("resources/ui/main.psoui");

    // Removes all ui elements and regenerate them
    ui_reloadUi();

    ui_renderer_init(uiElements);
}


void ui_reloadUi() {

    uiElements.clear();


    ui_createWidgets();

    ui_parseUiFileString();

}


/*
    // REFACTOR 2024-10-04
    Don't need an update function because the ui state is event driven.
    Instead we simply need to change this to a rendering  method that renderes the app state every frame.
*/
void ui_update() {
    // make sure all ui rendering is rendered on top of 3d scene
    glDisable(GL_DEPTH_TEST);

    // check if cursor is hovering over ui elements (post refactoring-elements)
    // std::cout << "UI::cursor_x=" << UI::cursor_x << ", UI::cursor_y=" << UI::cursor_y << "\n";
    // for (UI::List* _uiList : uiLists) {
    //     // Will remove transprancy during hover
    //     _uiList->containingPrimitive->containsPoint(UI::cursor_x, UI::cursor_y);
    // }

    // UI::setCurrentlyHoveredPrimitive();



    // REFACTORING - 2024-10-04
    // MAKE SURE TO RENDER ON TOP OF 3D SCENE
    // glDisable(GL_DEPTH_TEST);
    // for(UI::List* _uiList : uiLists){
    //     _uiList->update();
    //     _uiList->render();
    // }


    ui_updateWidgets();


    ui_renderer_render(uiElements);
}





void ui_createWidgets() {
    // TIMER WIDGET
    UiElement fpsStringElement;
    fpsStringElement.name = "fps";
    fpsStringElement.activated = 1;
    uiElements.push_back(fpsStringElement);
    UiElement frameCountStringElement;
    frameCountStringElement.name = "frame_count";
    frameCountStringElement.activated = 1;
    uiElements.push_back(frameCountStringElement);
    UiElement worldTimeStringElement;
    worldTimeStringElement.name = "world_time";
    worldTimeStringElement.activated = 1;
    uiElements.push_back(worldTimeStringElement);
    UiElement epochTimeStringElement;
    epochTimeStringElement.name = "world_epoch";
    epochTimeStringElement.activated = 1;
    uiElements.push_back(epochTimeStringElement);


    // WORLD OBJECTS
    std::vector<PObject*> _worldObjects = ws_getWorldObjects();

    UiElement woHeaderStringElement;
    woHeaderStringElement.name = "WORLD_OBJECTS_HEADER";
    woHeaderStringElement.activated = 1;
    uiElements.push_back(woHeaderStringElement);
    for (PObject* _wo_p : _worldObjects) {
        PObject& _wo = *_wo_p;
        UiElement woStringElement;
        woStringElement.name = _wo.name;
        woStringElement.activated = 1;
        uiElements.push_back(woStringElement);

    }


    // World object tracking
    UiElement trackedWorldObjectStringElement;
    trackedWorldObjectStringElement.name = "trackedWorldObject";
    trackedWorldObjectStringElement.activated = 1;
    uiElements.push_back(trackedWorldObjectStringElement);
    UiElement twose_name;
    twose_name.name = "twose_name";
    twose_name.activated = 1;
    uiElements.push_back(twose_name);
}


void ui_updateWidgets() {
    // std::cout << "timerWidgetIsOn = " << timerWidgetIsOn << std::endl;
    if (timerWidgetIsOn)
        ui_updateTimerWidget();
    if (worldObjectWidgetIsOn)
        ui_updateWorldObjectWidget();
    if (trackedWorldObjectWidgetIsOn)
        ui_updateTrackedWorldObjectElement();
}


void ui_updateWorldObjectWidget() {
    std::vector<PObject*> _worldObjects = ws_getWorldObjects();

    int wo_count = 0;
    for (PObject* _wo_p : _worldObjects) {
        PObject& _wo = *_wo_p;
        // Update elements
        for (UiElement& _uiElem : uiElements) {


            if (_uiElem.name == _wo.name) {
                // std::cout << "_uiElem.name = " << _uiElem.name << std::endl;
                // std::cout << "FPS is active? : " << _uiElem.activated << std::endl;
                ui_updateStringUi(15, 0, windowHeight - 160 - (wo_count * 20), _wo.name, _uiElem);
                wo_count++;
            }
            if (_uiElem.name == "WORLD_OBJECTS_HEADER")
                ui_updateStringUi(15, 0, windowHeight - 140, "WORLD OBEJCTS", _uiElem);

        }

    }
}



void ui_updateTimerWidget() {
    // std::cout << "timerWidgetIsOn = " << timerWidgetIsOn << std::endl;

    // fps
    int fps = timing_current_fps();
    // fpsStringElements.clear();
    std::string fps_str = "FPS: ";
    fps_str.append(std::to_string(fps));

    // Frame Counter
    int frame_count = timing_currentFrameCount();
    frameCountStringElements.clear();
    std::string frameCount_str = "frames: ";
    frameCount_str.append(std::to_string(frame_count));

    // world Time
    double world_time = timing_getWorldTime();
    worldTimeStringElements.clear();
    std::string worldTime_str = "W. time : ";
    std::string worldTime_cast = std::to_string(world_time);
    worldTime_cast.pop_back();
    worldTime_cast.pop_back();
    worldTime_cast.pop_back();
    // worldTime_str = std::to_string(world_time);
    worldTime_str.append(worldTime_cast);

    // World Epoch
    double world_epoch = timing_getWorldEpoch();
    worldEpochStringElements.clear();
    std::string worldEpoch_str = "W. epoch: ";
    std::string worldEpoch_cast = std::to_string(world_epoch);
    worldEpoch_cast.pop_back();
    worldEpoch_cast.pop_back();
    worldEpoch_cast.pop_back();
    worldEpoch_str.append(worldEpoch_cast);


    // Update elements
    for (UiElement& _uiElem : uiElements) {
        if (_uiElem.name == "fps") {
            // std::cout << "FPS is active? : " << _uiElem.activated << std::endl;
            ui_updateStringUi(15, 0, windowHeight - 45, fps_str, _uiElem);
        }
        else if (_uiElem.name == "frame_count") {
            // std::cout << "FPS is active? : " << _uiElem.activated << std::endl;
            ui_updateStringUi(15, 0, windowHeight - 65, frameCount_str, _uiElem);
        }
        else if (_uiElem.name == "world_time") {
            // std::cout << "FPS is active? : " << _uiElem.activated << std::endl;
            ui_updateStringUi(15, 0, windowHeight - 85, worldTime_str, _uiElem);
        }
        else if (_uiElem.name == "world_epoch") {
            // std::cout << "UiElement.elementType = " << _uiElem.elementType << std::endl;
            ui_updateStringUi(15, 0, windowHeight - 105, worldEpoch_str, _uiElem);
        }
    }


}

int trackedWorldObjectUpdateCount = 0;
void ui_updateTrackedWorldObjectElement() {

    // fpsStringElements.clear();

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
            ui_updateStringUi(15, 0, 300, worldObjectTracked_str, _uiElem);

        if (_uiElem.name == "twose_name") {
            // make sure element has correct type, even when not rendered for multiple frames
            _uiElem.elementType = ElementType::STRING;

            // Slow down updates
            if (trackedWorldObjectUpdateCount % 5 == 0) {
                ui_updateStringUi(15, 0, 278, twose_z_str, _uiElem);
                // std::cout << "trackedWorldObjectUpdateCount = " << trackedWorldObjectUpdateCount << std::endl;

            }
            trackedWorldObjectUpdateCount++;
        }
    }
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

        uiFile_main.fileContents = psouiString;

        psouiFile.close();


        std::cout << "PSUI OK. " << uiFilePath << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << " ERROR. " << uiFilePath << " : tried loading psoui file. " << std::endl;
        std::cout << "ERROR::READING_PSOUI_FILE" << e.what() << std::endl;
    }


}




void ui_parseUiFileString() {

    std::stringstream psouiStream;
    psouiStream.str(uiFile_main.fileContents);


    std::string keyValString;
    std::string key;
    std::string value;
    std::string line;



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


    if (_texturePath != "") {
        ui_renderer_setTexture(uiElem_, _texturePath);

    }

    // uiElem_.vertices
    uiElements.push_back(uiElem_);
}




void ui_updateStringUi(int fontHeight, int x, int y, std::string str, UiElement& _uiStringElem) {
    // UiElement _uiStringElem;
    // _uiStringElem.activated = 1;
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




void ui_setWindowSize(int _width, int _height) {
    windowWidth = _width;
    windowHeight = _height;

    uiTransform16[0] = 2.0f / windowWidth;
    uiTransform16[5] = 2.0f / windowHeight;

    UI::setViewportDimensions(_width, _height);

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







