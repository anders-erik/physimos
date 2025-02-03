#ifndef UI_WORLDOBJECT_HH
#define UI_WORLDOBJECT_HH

#include "WorldObject.hpp"

#include "ui/ui_primitive.hh"


namespace UI::WorldObject {

    // Setting to public to be able to use primitive base pointer without cast
    class ToggleWireframe : public UI::Primitive {
        public:
            ::WorldObject* worldObject = nullptr;
            void click();
    };


    Primitive* newComponent(::WorldObject* _worldObject);
}


#endif