#ifndef UI_MODULE_HH
#define UI_MODULE_HH


namespace UI {

    typedef enum ModuleType {
        WOList = 0,
    } ModuleType;

    struct UiModuleInfo {

    };


    class UiModule {
        UiModule() {};
        UiModule(ModuleType moduleType);

        void render();
    
    
    
    };


}


#endif