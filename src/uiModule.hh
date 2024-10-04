#ifndef UI_MODULE_HH
#define UI_MODULE_HH

#include <vector>

namespace UI {

    typedef enum ModuleType {
        WOToggleList = 0,
    } ModuleType;

    struct ModuleInfo {
        ModuleType moduleType;
        int height = 0;
        int width = 0;
        int x = 0;
        int y = 0;

        // // IMPLEMENT - REFACTOR
        // UiPrimitive primitiveContainer;
        // std::vector<UiUnit*> uiUnits;
    };


    class Module {
        public:
            Module() {};
            Module(ModuleType moduleType);

            void init(ModuleInfo* _moduleInfo);

            ModuleInfo* moduleInfo = nullptr;

            void render();
            void updatemoduleInfo(ModuleInfo* _moduleInfo);
        
        
        
    };


}


#endif