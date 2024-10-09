
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ui_module.hh"

#include "ui_primitive.hh"

namespace UI {


Module::Module(ModuleType _moduleType, std::string _name) {

    init(_moduleType, _name);
}

void Module::init(ModuleType _moduleType, std::string _name) {
    name = _name;

    if(_moduleType == ModuleType::List){
        initWoListModule();
    }
}

void Module::update() {
    containingPrimitive->update();

}

void Module::render() {
    // std::cout << "RENDERING UI MODULE" << std::endl;

    /*
        RENDER MODULE CONTAINER
    */
    containingPrimitive->render();


    /*
         RENDER MODULE UNITS
    */



}

void Module::initWoListModule() {
    std::cout << "NEW NEW UI MODULE" << std::endl;

    // Primitive
    PrimitiveInfo primitiveInfo;

    primitiveInfo.height = 400;
    primitiveInfo.width = 300;
    primitiveInfo.x = 400;
    primitiveInfo.y = 200;

    primitiveInfo.R = 0;
    primitiveInfo.G = 0;
    primitiveInfo.B = 0;
    primitiveInfo.A = 1;

    primitiveInfo.parent = nullptr;
    

    containingPrimitive = new Primitive(&primitiveInfo);


}




std::vector<Module*> uiModules;

std::vector<Module*>& loadModulesFromFile(){


    std::string moduleFilePath = "resources/ui/modules.pui";

    std::ifstream modulesFile;
    std::stringstream modulesStream;
    std::string modulesString;

    modulesFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {

        modulesFile.open(moduleFilePath);


        modulesStream << modulesFile.rdbuf();

        modulesString = modulesStream.str();

        // moduleFilePath.fileContents = psouiString;

        modulesFile.close();


        std::cout << "UI MOdules loaded without error. " << moduleFilePath << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;

        

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << " ERROR. " << moduleFilePath << " : tried loading psoui file. " << std::endl;
        std::cout << "ERROR::READING_PSOUI_FILE" << e.what() << std::endl;
    }



    /* 
        Step through all lines
    */
    std::string line;
    bool readingModuleValues = false;
    // std::map<std::string, std::string> moduleKVs;
    Module* module = nullptr;
    while (std::getline(modulesStream, line)) {

        // New module
        if (line.substr(0, 6) == "module") {
            std::cout << "MODULE START" << std::endl;
            readingModuleValues = true;
            module = new Module();
            module->containingPrimitive = new Primitive();
            uiModules.push_back(module);
            // moduleKVs.clear();
            continue;
        }
        // End of module
        else if(line.substr(0, 2) == "__"){
            std::cout << "MODULE END" << std::endl;
            readingModuleValues = false;


            // print
            // std::cout << "Module: " << moduleKVs.find("name") << std::endl;
            // for (const auto& [key, value] : moduleKVs)
            //     std::cout << '[' << key << "] = " << value << "; \n";
        }
        // End of file
        else if (line.substr(0, 2) == "XX") {
            std::cout << "END OF MODULE FILE" << std::endl;
            break;
        }


        // Grab a KV-line, parse, and add to module
        if(readingModuleValues){

            addToUiModule(line, module);
            
        }


        // std::cout << "" << line << std::endl;

    }


    return uiModules;
}

/** Add a value for corresponding key oto module property*/
void addToUiModule(std::string _KVstring, Module* _module){
    std::string key = _KVstring.substr(0, _KVstring.find(':'));
    key = str_trim(key);
    std::string value = _KVstring.substr(_KVstring.find(':') + 1, _KVstring.size() - 1);
    std::cout << "key=" << key ;
    std::cout << ",value=" << value << std::endl;
    // moduleKVs[key] = value;
    
    if (key == "ModuleType")
        _module->moduleType = getModuleType(value);
    else if (key == "name")
        _module->name = value;
    else if (key == "height")
        _module->containingPrimitive->height = std::stoi(value);
    else if (key == "width")
        _module->containingPrimitive->width = std::stoi(value);
    else if (key == "x")
        _module->containingPrimitive->x = std::stoi(value);
    else if (key == "y")
        _module->containingPrimitive->y = std::stoi(value);

}

ModuleType getModuleType(std::string _moduleTypeStr){
    if(_moduleTypeStr == "List")
        return ModuleType::List;
    
    return ModuleType::List;
}

/** Removes spaces and tabs before and after the first/last non-whitespace character */
std::string str_trim(std::string _str) {
    // std::cout << "TRIM START" << std::endl;

    std::string strTemp = _str;

    for (int i = 0; i < strTemp.length(); i++) {
        if (strTemp[i] != ' ' && strTemp[i] != '\t') {
            strTemp = strTemp.substr(i, strTemp.length());
            break;
        }

    }
    for (int i = strTemp.length() - 1; i >= 0; i++) {
        if (strTemp[i] != ' ' && strTemp[i] != '\t') {
            strTemp = strTemp.substr(0, i+1);
            break;
        }
    }
    // std::cout << "TRIM START" << std::endl;

    return strTemp;
}


// Module* grabNextModule(){

// }


}



