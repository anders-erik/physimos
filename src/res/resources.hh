#ifndef RESOURCES_HH
#define RESOURCES_HH

#include <map>
#include <vector>

#include <string>

namespace res {


    typedef enum ModelFileType {
        unknownFileType,
        obj,
        pso,
    } ModelType;

    typedef enum ModelVertStucture {
        unknownVertStruct,
        p3n3t2,
        p3c3,
        p3c3t2,
        p3t2,
    } ModelVertStucture;

    typedef struct ModelFormat {
        std::string name = "";
        ModelFileType fileType = ModelFileType::unknownFileType;
        ModelVertStucture vertStructure = ModelVertStucture::unknownVertStruct;

    } ModelFormat;



    void createInventory();
    ModelFormat* getModelFormat(std::string modelName);

}



#endif