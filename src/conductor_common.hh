#ifndef CONDUCTOR_COMMON_HH
#define CONDUCTOR_COMMON_HH


typedef enum class StateMain {
    Scene3D = 0,
    Canvas = 1,
    UIEditor = 2, 
} StateMain;

typedef enum class CAction {
    None,

    UI_ToggleWorkbench,
    UI_ToggleLeftPanel,
    UI_ToggleRightPanel,

    State_ToggleScene3D,
    State_ToggleCanvas,
    State_ToggleUIEditor,
    State_SwitchRight,
    State_SwitchLeft,
} CAction;


#endif