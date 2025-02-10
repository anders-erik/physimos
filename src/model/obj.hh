#ifndef OBJ_HH
#define OBJ_HH

#include <string>
#include <vector>
#include <filesystem>

#include "obj_types.hh"

std::vector<std::string> split_str(std::string str, char delimiter);


namespace pmodel {
namespace pobj {


    typedef enum ObjLoadStatus {
        Ok = 1,
        ObjPathError = 20,
        MtlPathError = 21,
        FileFormatError = 23,
        ParsingError = 24,
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
        std::vector<Mtl*> objMtls;
        Mtl* currentMtl;
        // populates
        ObjLoadStatus loadMtlFile();

        // Vertex Data
        std::vector<VertexCoord> vertCoords;
        std::vector<VertexNormal> vertNormals;
        std::vector<VertexTextureCoord> vertTextureCoords;
        ObjLoadStatus processVertexCoordinateLine(std::string line);
        ObjLoadStatus processVertexTextureLine(std::string line);
        ObjLoadStatus processVertexNormalLine(std::string line);
        
        // A vector containing all triangular faces of the mesh.
        // Each face is represented by vertex indexes.
        std::vector<TriangleFaceI> triangleFacesI;
        ObjLoadStatus processFaceLineDataSegments(std::string line);

        // loads obj & mtl file into memory.
        // Requires that a valid path to the .obj file has been set using 'setModelPaths()'.
        ObjLoadStatus loadObjFile();

    } Obj;

}
}


#endif