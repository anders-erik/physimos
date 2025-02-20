
#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <string>
#include <vector>

#include "Windowing.hpp"
#include "Input.hpp"

#include "uiElement.hpp"
#include "Types.hpp"

#include "ui/ui_primitive.hh"
#include "ui/ui_globals.hh"

// #include "ui_old.hpp"



namespace UI {

    void init();
    void update();

    
    
    void window_changed_callback(PhysWin physimos_window); 

    UI::Action updatePObjectPosition();

    Primitive* getTargetingPrimitive();

    void setCurrentlyHoveredPrimitive(Primitive* primitiveToHover);


    // INTERNAL CALLBACK METHODS

    void pointerPositionCallback(PInput::PointerPosition pointer_pos, PInput::PointerChange _pointer_change);
    void leftClickCallback(PInput::PointerPosition _pointer_pos);
    void leftReleaseCallback(PInput::PointerPosition _pointer_pos);
    void scrollyCallback(double y_change);
    void windowSizeChangeCallback(double x, double y);
}



#endif