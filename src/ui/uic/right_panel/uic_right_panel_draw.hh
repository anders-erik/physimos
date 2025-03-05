#ifndef UIC_RIGHT_PANEL_DRAW_HH
#define UIC_RIGHT_PANEL_DRAW_HH


#include "ui/ui_primitive.hh"

#include "ui/uic/draw/uic_draw_transform.hh"
#include "ui/uic/draw/uic_draw_brush.hh"


namespace UI::component {




struct UIC_RightPanel_Draw: public ::UI::Primitive {


    UIC_Draw_Transform transform;
    UIC_Draw_Brush brush;
    

    void update_component();
    void render_component();

    UiResult try_find_target_component(double x, double y);
    
    UIC_RightPanel_Draw();
};




}


#endif