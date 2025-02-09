#ifndef OBJ__HH
#define OBJ__HH

#include <string>
#include <vector>
#include <filesystem>

#include "obj_types.hh"

namespace pmodel {


    // Loaded obj-model. Only supports triangular faces. 
    typedef struct Obj {
        std::string modelName = "";
        std::filesystem::path modelFilePath;

        std::vector<VertexCoord> vertCoords;
        std::vector<VertexNorm> vertNormals;
        std::vector<VertexTextCoord> vertexTextureCoords;
        
         

    } Obj;


    /*
        MTL
    */


    float* obj_loadKdFromFile(std::string mtlPath);

    void obj_loadMtlFromFile(std::string mtlPath);



    /*
        OBJ
    */

    std::vector<float> obj_getVertexBuffer_v_vt_vn();
    void obj_loadFromFile(std::string modelName);

}


#endif