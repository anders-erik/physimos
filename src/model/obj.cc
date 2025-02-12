
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "lib/string.hh"

#include "obj.hh"
#include "obj_types.hh"

#include "res/bmp_loader.hpp"

// using namespace plib;


namespace pmodel {
namespace pobj {


// Represents value of texture coordinate that was not present in file
float emptyTextureCoord = -2222.2;


// UTIL
std::string modelsDirectory = "resources/models/";

// MTL
bool hasTextureMap = false;
float Kd[3] = { 0.0f, 0.0f, 0.0f };
std::string texturePath = "";

std::vector<unsigned char> obj_textureDataBuffer;
unsigned int obj_imgWidth = 0;
unsigned int obj_imgHeight = 0;


ObjLoadStatus Obj::setModelPaths(std::filesystem::path _modelFilePath) {
    // std::filesystem::path filePath (_modelFilePath);

    if (!std::filesystem::exists(_modelFilePath)) {
        return ObjLoadStatus::ObjPathError;
    }

    modelFilePath = _modelFilePath;
    modelFileDir = modelFilePath.parent_path();
    modelFileName = modelFilePath.filename();
    modelName = std::string( modelFileName.stem() );
    // std::cout << "modelFilePath = " << modelFilePath << std::endl;
    // std::cout << "modelFileName = " << modelFileName << std::endl;
    // std::cout << "modelName = " << modelName << std::endl;

    return ObjLoadStatus::Ok;
}

ObjLoadStatus Obj::loadMtlFile(){

    // Load file content into stream
    std::ifstream mtlFile;
    std::stringstream mtlStream;
    mtlFile.open(mtlFilePath);
    mtlStream << mtlFile.rdbuf();
    mtlFile.close();

    std::vector<std::string> mtlFileLines = plib::std_string::split(mtlStream.str(), '\n');
    
    for(std::string mtlFileLine : mtlFileLines){
        // empty lines are immediately skipped
        if (mtlFileLine.size() < 1)
            continue;

        std::vector<std::string> line_segments = plib::std_string::split(mtlFileLine, ' ');

        // this should always be encountered first, and triggers the creation of a new mtl-object.
        if (line_segments[0] == "newmtl") {
            currentMtl = objMtls.emplace_back(new Mtl());
            currentMtl->name = line_segments[1];
        }
        else if (line_segments[0] == "map_Kd") {
            currentMtl->map_Kd = line_segments[1];
        }
        else if (line_segments[0] == "Ka") {
            currentMtl->Ka.R = (float)std::atof(line_segments[1].data());
            currentMtl->Ka.G = (float)std::atof(line_segments[2].data());
            currentMtl->Ka.B = (float)std::atof(line_segments[3].data());
        }
        else if (line_segments[0] == "Ks") {
            currentMtl->Ks.R = (float)std::atof(line_segments[1].data());
            currentMtl->Ks.G = (float)std::atof(line_segments[2].data());
            currentMtl->Ks.B = (float)std::atof(line_segments[3].data());
        }
        else if (line_segments[0] == "Kd") {
            currentMtl->Kd.R = (float)std::atof(line_segments[1].data());
            currentMtl->Kd.G = (float)std::atof(line_segments[2].data());
            currentMtl->Kd.B = (float)std::atof(line_segments[3].data());
        }
        else if (line_segments[0] == "Ke") {
            currentMtl->Ke.R = (float)std::atof(line_segments[1].data());
            currentMtl->Ke.G = (float)std::atof(line_segments[2].data());
            currentMtl->Ke.B = (float)std::atof(line_segments[3].data());
        }
        else if (line_segments[0] == "Ns") {
            currentMtl->Ns = (float)std::atof(line_segments[1].data());
        }
        else if (line_segments[0] == "Ni") {
            currentMtl->Ni = (float)std::atof(line_segments[1].data());
        }
        else if (line_segments[0] == "d") {
            currentMtl->d = (float)std::atof(line_segments[1].data());
        }
        else if (line_segments[0] == "illum") {
            currentMtl->illum = (float)std::atof(line_segments[1].data());
        }
    }
    

    return ObjLoadStatus::Ok;
}

ObjLoadStatus Obj::loadObjFile() {
    if (!std::filesystem::exists(modelFilePath)){
        std::cout << "modelFilePath = " << modelFilePath << std::endl;
        
        return ObjLoadStatus::ObjPathError;
    }

    // Read Whole Obj File
    std::ifstream modelFile;
    std::stringstream modelStream;

    modelFile.open(modelFilePath);
    modelStream << modelFile.rdbuf();
    modelFile.close();

    // objFileContents = modelStream.str();


    // Parse file
    std::vector<std::string> objFileLines = plib::std_string::split(modelStream.str(), '\n');

    for(std::string line : objFileLines){
        // std::cout << "" << line << std::endl;
        std::vector<std::string> line_segments = plib::std_string::split(line, ' ');

        if (line_segments[0] == "v"){
            // std::cout << "v" << std::endl;
            processVertexCoordinateLine(line);
        }   
        else if (line_segments[0] == "vt") {
            // std::cout << "vt" << std::endl;
            processVertexTextureLine(line);
        }
        else if (line_segments[0] == "vn") {
            // std::cout << "vn" << std::endl;
            processVertexNormalLine(line);
        }
        else if (line_segments[0] == "f") {
            // std::cout << "f" << std::endl;
            processFaceLineDataSegments(line);
        }
        else if (line_segments[0] == "usemtl") { // set the material that will be applied to the following faces
            std::string mtlName = line_segments[1];
            // Find and set the current material
            for(Mtl* mtl : objMtls){
                if (mtl->name == mtlName){
                    currentMtl = mtl;
                    break;
                }
            }
        }
        else if (line_segments[0] == "mtllib") { // Should be run BEFORE any other lines!
            mtlFilePath = modelFileDir.append(line_segments[1]);
            if (!std::filesystem::exists(mtlFilePath)) {
                return ObjLoadStatus::MtlPathError;
            }
            loadMtlFile();
        }
    }
    


    return ObjLoadStatus::Ok;
}


ObjLoadStatus Obj::processVertexCoordinateLine(std::string line) {
    // std::cout << "" << (float)std::atof(lineDataSegments[1].data()) << std::endl;

    std::vector<std::string> line_segments = plib::std_string::split(line, ' ');

    float x = (float)std::atof(line_segments[1].data());
    float y = (float)std::atof(line_segments[2].data());
    float z = (float)std::atof(line_segments[3].data());

    VertexCoord vertexCoord = { x, y, z, 1.0,};

    // The w value is optional
    if (line_segments.size() > 4)
        vertexCoord.w = (float)std::atof(line_segments[4].data());

    vertCoords.push_back(vertexCoord);

    return ObjLoadStatus::Ok;
}


ObjLoadStatus Obj::processVertexTextureLine(std::string line) {
    std::vector<std::string> line_segments = plib::std_string::split(line, ' ');

    float u = (float)std::atof(line_segments[1].data());
    float v = (float)std::atof(line_segments[2].data());

    VertexTextureCoord vertexTexture = {u, v, emptyTextureCoord};

    if (line_segments.size() > 3)
        vertexTexture.w = (float)std::atof(line_segments[3].data());

    vertTextureCoords.push_back(vertexTexture);
    return ObjLoadStatus::Ok;
}

ObjLoadStatus Obj::processVertexNormalLine(std::string line) {
    std::vector<std::string> line_segments = plib::std_string::split(line, ' ');

    float x = (float)std::atof(line_segments[1].data());
    float y = (float)std::atof(line_segments[2].data());
    float z = (float)std::atof(line_segments[3].data());

    VertexNormal vertexNormal = {x, y, z};

    vertNormals.push_back(vertexNormal);

    return ObjLoadStatus::Ok;
}

ObjLoadStatus Obj::processFaceLineDataSegments(std::string line) {
    std::vector<std::string> line_segments = plib::std_string::split(line, ' ');
    
    std::vector <std::string> vert1_index_strings = plib::std_string::split(line_segments[1], '/');
    std::vector <std::string> vert2_index_strings = plib::std_string::split(line_segments[2], '/');
    std::vector <std::string> vert3_index_strings = plib::std_string::split(line_segments[3], '/');

    TriangleFaceI triangleFaceI;
    
    // vertex_index/texture_index/normal_index
    VertexI vertexi_1;
    vertexi_1.vc_i = std::atoi(vert1_index_strings[0].data());
    vertexi_1.vt_i = std::atoi(vert1_index_strings[1].data());
    vertexi_1.vn_i = std::atoi(vert1_index_strings[2].data());

    VertexI vertexi_2;
    vertexi_2.vc_i = std::atoi(vert2_index_strings[0].data());
    vertexi_2.vt_i = std::atoi(vert2_index_strings[1].data());
    vertexi_2.vn_i = std::atoi(vert2_index_strings[2].data());

    VertexI vertexi_3;
    vertexi_3.vc_i = std::atoi(vert3_index_strings[0].data());
    vertexi_3.vt_i = std::atoi(vert3_index_strings[1].data());
    vertexi_3.vn_i = std::atoi(vert3_index_strings[2].data());

    triangleFaceI.v1 = vertexi_1;
    triangleFaceI.v2 = vertexi_2;
    triangleFaceI.v3 = vertexi_3;

    triangleFaceI.mtl = currentMtl;

    triangleFacesI.push_back(triangleFaceI);

    return ObjLoadStatus::Ok;
}




} // end obj namespace
} // end pmodel namespace