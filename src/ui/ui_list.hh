#ifndef UI_MODULE_HH
#define UI_MODULE_HH

#include <vector>
#include <string>

#include "ui_primitive.hh"

namespace UI {

class List;

typedef enum ListType {
    Stack = 1,
} ListType;


// UTILITES
std::string str_trim(std::string _str);
void addToUiList(std::string _KVstring, List* list);
ListType getListType(std::string _listTypeStr);





class List {
    public:
        List() {};
        List(ListType _listType, std::string _name);
        // List(PrimitiveInfo* _primitiveInfo_ptr);
        void init(ListType _listType, std::string _name);

        // void init(PrimitiveInfo* _primitiveInfo_ptr);
        void initWoListList();

        std::string name;

        ListType listType;

        // Primitive* listInfo_ptr = nullptr;

        // The 'outline' of the list
        Primitive* containingPrimitive = nullptr;


        void update();
        void render();
};



std::vector<List*>& loadListsFromFile();



}


#endif