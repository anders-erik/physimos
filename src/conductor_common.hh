#ifndef CONDUCTOR_COMMON_HH
#define CONDUCTOR_COMMON_HH


typedef enum class StateMain {
    Scene3D = 0,
    Draw = 1,
    UIEditor = 2, 
} StateMain;

typedef enum class CAction : unsigned int {

    UI_MIN = 0,
    UI_ToggleWorkbench = 1,
    UI_ToggleLeftPanel = 2,
    UI_ToggleRightPanel = 3,
    UI_MAX = 63,

    State_ToggleScene3D,
    State_ToggleCanvas,
    State_ToggleUIEditor,
    State_SwitchRight,
    State_SwitchLeft,

    None,
} CAction;


#endif