#ifndef UI_PSCENE_HH
#define UI_PSCENE_HH

#include "pscene.hh"
#include "WorldObject.hpp"
// #include "Scene.hh"

#include "ui/ui_primitive.hh"


namespace UI::PScene {

    // Primitive with a WorldObject pointer. 
    struct Base : public UI::Primitive {
        ::PScene::Scene* pObject = nullptr;
        
        Base(){};
        
        virtual void reload() {};
    };


    struct Container : public UI::PScene::Base {
        Container();
    };


    struct NameLabel : public UI::PScene::Base {
        NameLabel(std::string sceneName);
    };


    struct PObjectListObject : public UI::PScene::Base {
        ::WorldObject* pObject = nullptr;

        PObjectListObject(::WorldObject* _pObject);
        
        // will reload the pobject container with pobject currently pointed to by this primitive
        ::UI::Action click() {
            return ::UI::Action::LoadPObject;
        };
    };

    struct PObjectList : public UI::PScene::Base {
        std::vector<::WorldObject*> pObjects;
        
        PObjectList();

        void addPObject(::WorldObject* _pObject);
        void removePObject(::WorldObject* _pObject);
        void reloadList();
    };




    // Full context ui component for PObjects (formerly WorldObject)
    class Context {
    public:
        ::PScene::Scene* scene = nullptr;

        Container* container = nullptr;
        NameLabel* nameLabel = nullptr;
        PObjectList* pObjectList = nullptr;

        Context() {};
        
        void populateContext(::PScene::Scene* _scene);
        // void newPObject(::WorldObject* _pObject);

        // void reloadComponent();

    };

    extern Context* uiPSceneContext;
}


#endif