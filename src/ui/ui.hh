
#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <string>
#include <vector>


#include "uiElement.hpp"
#include "Types.hpp"

#include "ui/ui_primitive.hh"
#include "ui/ui_globals.hh"

// #include "ui_old.hpp"



namespace UI {

    void init();
    void update();

    void pointerPositionCallback(double x, double y);
    void leftClickCallback(double x, double y);
    void windowSizeChangeCallback(double x, double y);

    UI::Action updatePObjectPosition();

    Primitive* getTargetingPrimitive();

    void setCurrentlyHoveredPrimitive(Primitive* primitiveToHover);

}



#endif