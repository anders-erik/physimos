#ifndef UI_MODULE_HH
#define UI_MODULE_HH

#include <vector>
#include <string>

#include "ui_primitive.hh"

namespace UI {

class Module;

typedef enum ModuleType {
    List = 1,
} ModuleType;


// UTILITES
std::string str_trim(std::string _str);
void addToUiModule(std::string _KVstring, Module* module);
ModuleType getModuleType(std::string _moduleTypeStr);



std::vector<Module*>& loadModulesFromFile();




class Module {
    public:
        Module() {};
        Module(ModuleType _moduleType, std::string _name);
        // Module(PrimitiveInfo* _primitiveInfo_ptr);
        void init(ModuleType _moduleType, std::string _name);

        // void init(PrimitiveInfo* _primitiveInfo_ptr);
        void initWoListModule();

        std::string name;

        ModuleType moduleType;

        // Primitive* moduleInfo_ptr = nullptr;

        Primitive* containingPrimitive = nullptr;


        void update();
        void render();
};


}


#endif