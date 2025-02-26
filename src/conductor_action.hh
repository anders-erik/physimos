#ifndef CONDUCTOR_ACTION_HH
#define CONDUCTOR_ACTION_HH

typedef enum class CAction {
    None,
    UI_ToggleWorkbench,
    UI_ToggleLeftPanel,
    UI_ToggleRightPanel,
    State_ToggleScene3D,
    State_ToggleCanvas,
    State_ToggleUIEditor,
} CAction;

#endif