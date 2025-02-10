#ifndef OBJ_HH
#define OBJ_HH

#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

#include "obj_types.hh"

std::vector<std::string> split_str(std::string str, char delimiter);


namespace pmodel {

namespace pobj {

    typedef enum ObjLoadStatus {
        Ok = 0,

        PathError = 20,
        FileFormatError = 21,
        ParsingError = 22,
    } ObjLoadStatus;



    // Loaded obj-model. Only supports triangular faces. 
    typedef struct Obj {

        std::string modelName = "";
        std::filesystem::path modelFileDir = "";
        std::filesystem::path modelFileName = "";
        std::filesystem::path modelFilePath = "";
        ObjLoadStatus setModelPaths(std::filesystem::path _modelFilePath);

        // Mtl file
        std::filesystem::path mtlFilePath = "";
        // List of defined materials in the mtl file
        std::vector<Mtl> objMtls;
        Mtl currentMtl;
        ObjLoadStatus loadMtlFile();

        // A vector containing all triangular faces of the mesh.
        // Each face is represented by vertex indexes.
        std::vector<TriangleFaceI> triangleFacesI;
        
        std::vector<VertexCoord> vertCoords;
        std::vector<VertexNormal> vertNormals;
        std::vector<VertexTextureCoord> vertTextureCoords;
        
        ObjLoadStatus loadObjFile();

        ObjLoadStatus processVertexCoordinateLine(std::string line);
        ObjLoadStatus processVertexTextureLine(std::string line);
        ObjLoadStatus processVertexNormalLine(std::string line);

        ObjLoadStatus processFaceLineDataSegments(std::string line);


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
}


#endif