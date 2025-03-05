#ifndef UIC_DRAW_BRUSH_HH
#define UIC_DRAW_BRUSH_HH


#include "ui/ui_primitive.hh"

#include "ui/ui_globals.hh"

#include "draw/draw.hh"


namespace UI::component {


struct UIC_Draw_Brush_SetGreen : public ::UI::Primitive {
    UiResult click();
    UIC_Draw_Brush_SetGreen();
};
struct UIC_Draw_Brush_SetRed : public ::UI::Primitive {
    UiResult click();
    UIC_Draw_Brush_SetRed();
};
struct UIC_Draw_Brush_DecSize : public ::UI::Primitive {
    UiResult click();
    UIC_Draw_Brush_DecSize();
};
struct UIC_Draw_Brush_IncSize : public ::UI::Primitive {
    UiResult click();
    UIC_Draw_Brush_IncSize();
};
struct UIC_Draw_Brush_SizeLabel : public ::UI::Primitive {
    void update_component();
    UIC_Draw_Brush_SizeLabel();
};


struct UIC_Draw_Brush : public ::UI::Primitive {

    draw::Brush& boundObject;

    Primitive title;

    UIC_Draw_Brush_SetGreen set_green_btn;
    UIC_Draw_Brush_SetRed   set_red_btn;

    UIC_Draw_Brush_DecSize  dec_size;
    UIC_Draw_Brush_IncSize  inc_size;
    UIC_Draw_Brush_SizeLabel size_label;

    UiResult try_find_target_component(double x, double y);


    void update_component();
    void render_component();
    
    UIC_Draw_Brush();
};



}


#endif