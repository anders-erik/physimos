
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ui_list.hh"

#include "ui_primitive.hh"

namespace UI {


std::vector<List*> uiLists;



// List::List(ListType _listType, std::string _name) {

//     init(_listType, _name);
// }

// void List::init(ListType _listType, std::string _name) {
//     name = _name;

//     if (_listType == ListType::Stack) {
//         initWoListList();
//     }
// }

void List::update() {
    containingPrimitive->updateTransformsRecursive();

}

void List::render() {
    // std::cout << "RENDERING UI LIST" << std::endl;

    /*
        RENDER LIST CONTAINER
    */
    containingPrimitive->render();


    /*
         RENDER LIST UNITS
    */



}

void List::initWoListList() {
    std::cout << "NEW NEW UI LIST" << std::endl;

    Primitive* _primitive = new Primitive();

    // _primitive->x_real = 400; // readonly
    // _primitive->y_real = 200; // readonly


    _primitive->width = 200;
    _primitive->height = 400;
    // _primitive->width = 64;
    // _primitive->height = 24;

    _primitive->horiRef = HoriRef::Right;
    _primitive->x_unit = Unit::Percent;
    _primitive->setX(30);
    _primitive->vertRef = VertRef::Top;
    _primitive->y_unit = Unit::Percent;
    _primitive->setY(20);


    _primitive->R = 100;
    _primitive->G = 100;
    _primitive->B = 100;
    _primitive->A = 150;

    // _primitive->parent = nullptr;

    _primitive->initGraphics();
    // _primitive->setString("AAAA");
    containingPrimitive = _primitive;


    // // // Primitive
    // PrimitiveInfo primitiveInfo;

    // primitiveInfo.height = 400;
    // primitiveInfo.width = 200;
    // primitiveInfo.x = 400;
    // primitiveInfo.y = 200;

    // primitiveInfo.R = 255;
    // primitiveInfo.G = 0;
    // primitiveInfo.B = 0;
    // primitiveInfo.A = 127;

    // // This is a root ui element
    // primitiveInfo.parent = nullptr;
    

    // containingPrimitive = new Primitive(&primitiveInfo);

    // int a = 1;
}


std::vector<List*>& loadListsFromFile(){


    std::string listFilePath = "resources/ui/lists.pui";

    std::ifstream listsFile;
    std::stringstream listsStream;
    std::string listsString;

    listsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {

        listsFile.open(listFilePath);


        listsStream << listsFile.rdbuf();

        listsString = listsStream.str();

        // listFilePath.fileContents = psouiString;

        listsFile.close();


        std::cout << "UI MOdules loaded without error. " << listFilePath << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;

        

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << " ERROR. " << listFilePath << " : tried loading psoui file. " << std::endl;
        std::cout << "ERROR::READING_PSOUI_FILE" << e.what() << std::endl;
    }



    /* 
        Step through all lines
    */
    std::string line;
    bool readingListValues = false;
    // std::map<std::string, std::string> listKVs;
    List* list = nullptr;
    while (std::getline(listsStream, line)) {

        // New list
        if (line.substr(0, 6) == "list") {
            std::cout << "LIST START" << std::endl;
            readingListValues = true;
            list = new List();
            list->containingPrimitive = new Primitive();
            uiLists.push_back(list);
            // listKVs.clear();
            continue;
        }
        // End of list
        else if(line.substr(0, 2) == "__"){
            std::cout << "LIST END" << std::endl;
            readingListValues = false;


            // print
            // std::cout << "List: " << listKVs.find("name") << std::endl;
            // for (const auto& [key, value] : listKVs)
            //     std::cout << '[' << key << "] = " << value << "; \n";
        }
        // End of file
        else if (line.substr(0, 2) == "XX") {
            std::cout << "END OF LIST FILE" << std::endl;
            break;
        }


        // Grab a KV-line, parse, and add to list
        if(readingListValues){

            addToUiList(line, list);
            
        }


        // std::cout << "" << line << std::endl;

    }


    return uiLists;
}

/** Add a value for corresponding key oto list property*/
void addToUiList(std::string _KVstring, List* _list){
    std::string key = _KVstring.substr(0, _KVstring.find(':'));
    key = str_trim(key);
    std::string value = _KVstring.substr(_KVstring.find(':') + 1, _KVstring.size() - 1);
    std::cout << "key=" << key ;
    std::cout << ",value=" << value << std::endl;
    // listKVs[key] = value;
    
    if (key == "ListType")
        _list->listType = getListType(value);
    else if (key == "name")
        _list->name = value;
    else if (key == "height")
        _list->containingPrimitive->height = std::stoi(value);
    else if (key == "width")
        _list->containingPrimitive->width = std::stoi(value);
    else if (key == "x")
        _list->containingPrimitive->x_real = std::stoi(value);
    else if (key == "y")
        _list->containingPrimitive->y_real = std::stoi(value);

}

ListType getListType(std::string _listTypeStr){
    if(_listTypeStr == "List")
        return ListType::Stack;
    
    return ListType::Stack;
}

/** Removes spaces and tabs before and after the first/last non-whitespace character */
std::string str_trim(std::string _str) {
    // std::cout << "TRIM START" << std::endl;

    std::string strTemp = _str;

    for (int i = 0; i < strTemp.length(); i++) {
        if (strTemp[i] != ' ' && strTemp[i] != '\t') {
            strTemp = strTemp.substr(i, strTemp.length());
            break;
        }

    }
    for (int i = strTemp.length() - 1; i >= 0; i++) {
        if (strTemp[i] != ' ' && strTemp[i] != '\t') {
            strTemp = strTemp.substr(0, i+1);
            break;
        }
    }
    // std::cout << "TRIM START" << std::endl;

    return strTemp;
}


// List* grabNextList(){

// }


}



