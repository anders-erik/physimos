
#include <iostream>

#include "uiModule.hh"

// #include ""

namespace UI {


Module::Module(ModuleType moduleType) {
    
}

void Module::init(ModuleInfo* _moduleInfo) {
    std::cout << "NEW NEW UI MODULE" << std::endl;
    updatemoduleInfo(_moduleInfo);

}


void Module::updatemoduleInfo(ModuleInfo* _moduleInfo){
    moduleInfo = _moduleInfo;
}


void Module::render() {
    // std::cout << "RENDERING UI MODULE" << std::endl;

    /* 
        RENDER MODULE CONTAINER
    */


    /*
         RENDER MODULE UNITS
    */


}



}