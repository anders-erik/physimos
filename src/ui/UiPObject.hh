#ifndef UI_WORLDOBJECT_HH
#define UI_WORLDOBJECT_HH

#include "scene/pobject.hh"

#include "ui/ui_primitive.hh"


namespace UI::PObject {

    // Primitive with a WorldObject pointer. 
    struct Base : public UI::Primitive {
        ::PObject* pObject = nullptr;
        virtual void setPObject(::PObject* _pObject) {};
        virtual void reload() {};

        Base(){};
    };


    struct Container : public UI::PObject::Base {
        Container(::PObject* _pObject);
        void setPObject(::PObject* _pObject);
    };


    struct NameLabel : public UI::PObject::Base {
        NameLabel(::PObject* _pObject);
        void setPObject(::PObject* _pObject);
    };


    struct XPosition : public UI::PObject::Base {
        XPosition(::PObject* _pObject);
        void setPObject(::PObject* _pObject);
        void reload();
    };
    struct XPosIncrease : public UI::PObject::Base {
        XPosIncrease(::PObject* _pObject);
        void setPObject(::PObject* _pObject);
        UI::Action click();
    };
    struct XPosDecrease : public UI::PObject::Base {
        XPosDecrease(::PObject* _pObject);
        void setPObject(::PObject* _pObject);
        UI::Action click();
    };


    struct YPosition : public UI::PObject::Base {
        YPosition(::PObject* _pObject);
    };
    struct YPosIncrease : public UI::PObject::Base {
        YPosIncrease(::PObject* _pObject);
        UI::Action click();
    };
    struct YPosDecrease : public UI::PObject::Base {
        YPosDecrease(::PObject* _pObject);
        UI::Action click();
    };


    struct ZPosition : public UI::PObject::Base {
        ZPosition(::PObject* _pObject);
    };
    struct ZPosIncrease : public UI::PObject::Base {
        ZPosIncrease(::PObject* _pObject);
        UI::Action click();
    };
    struct ZPosDecrease : public UI::PObject::Base {
        ZPosDecrease(::PObject* _pObject);
        UI::Action click();
    };



    // Setting to public to be able to use primitive base pointer without cast
    class ToggleWireframe : public UI::PObject::Base {
    public:
        ToggleWireframe(::PObject* _pObject);
        void setPObject(::PObject* _pObject);
        UI::Action click();
    };





    // Primitive* newComponent(::WorldObject* _pObject);


    // Full context ui component for PObjects (formerly WorldObject)
    class Context {
    public:
        Container* container = nullptr;
        NameLabel* nameLabel = nullptr;
        XPosition* xPosition = nullptr;
        XPosIncrease* xPosIncrease = nullptr;
        XPosDecrease* xPosDecrease = nullptr;
        YPosition* yPosition = nullptr;
        YPosIncrease* yPosIncrease = nullptr;
        YPosDecrease* yPosDecrease = nullptr;
        ZPosition* zPosition = nullptr;
        ZPosIncrease* zPosIncrease = nullptr;
        ZPosDecrease* zPosDecrease = nullptr;
        ToggleWireframe* toggleWireframe = nullptr;

        Context() {};
        
        void populateContext(::PObject* _pObject);
        void newPObject(::PObject* _pObject);

        void reloadComponent();

    };

    extern Context* uiPObjectContext;
}


#endif