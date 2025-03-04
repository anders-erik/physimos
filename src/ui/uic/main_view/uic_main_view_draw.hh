#ifndef UIC_MAIN_VIEW_DRAW_HH
#define UIC_MAIN_VIEW_DRAW_HH


#include "ui/ui_primitive.hh"


namespace UI::component {

typedef struct UIC_MainView_Draw_CursorPos : public ::UI::Primitive {
    UIC_MainView_Draw_CursorPos();

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
} UIC_MainView_Draw_CursorPos;

struct UIC_MainView_Draw : public ::UI::Primitive {

    UIC_MainView_Draw_CursorPos uic_MainView_Draw_CursorPos;

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_MainView_Draw();
};




}


#endif