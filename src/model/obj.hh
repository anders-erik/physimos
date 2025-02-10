#ifndef OBJ__HH
#define OBJ__HH

#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

#include "obj_types.hh"

std::vector<std::string> split_str(std::string str, char delimiter);


namespace pmodel {



    typedef enum ObjLoadStatus {
        Ok = 0,

        PathError = 20,
        FileFormatError = 21,
        ParsingError = 22,
    } ObjLoadStatus;




    // Loaded obj-model. Only supports triangular faces. 
    typedef struct Obj {
        std::string modelName = "";
        std::filesystem::path modelFilePath = "";

        std::string objFileContents;

        std::vector<VertexCoord> vertCoords;
        std::vector<VertexNormal> vertNormals;
        std::vector<VertexTextureCoord> vertTextureCoords;
        
        // A vector containing all triangular faces of the mesh.
        // Each face is represented by vertex indexes.
        std::vector<TriangleFaceI> triangleFacesI;
        
        ObjLoadStatus loadObjFile();

        ObjLoadStatus processVertexCoordinateLine(std::string line);
        ObjLoadStatus processVertexTextureLine(std::string line);
        ObjLoadStatus processVertexNormalLine(std::string line);

        ObjLoadStatus processFaceLineDataSegments(std::string line);

        // ObjLoadStatus openFilestream();
        // ObjLoadStatus closeFilestream();

        // ObjLoadStatus closeFilestream();

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